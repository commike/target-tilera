
#if defined(CONFIG_USER_ONLY)
int cpu_tilera_handle_mmu_fault(CPUTileraState *env, target_ulong address,
                               int rw, int mmu_idx)
{
    env->exception_index = EXCP_MMFAULT;
    env->trap_arg0 = address;
    return 1;
}
#else

/* Returns the OSF/1 entMM failure indication, or -1 on success.  */
static int get_physical_address(CPUTileraState *env, target_ulong addr,
                                int prot_need, int mmu_idx,
                                target_ulong *pphys, int *pprot)
{
    target_long saddr = addr;
    target_ulong phys = 0;
    target_ulong L1pte, L2pte, L3pte;
    target_ulong pt, index;
    int prot = 0;
    int ret = MM_K_ACV;

    /* Ensure that the virtual address is properly sign-extended from
       the last implemented virtual address bit.  */
    if (saddr >> TARGET_VIRT_ADDR_SPACE_BITS != saddr >> 63) {
        goto exit;
    }

    /* Translate the superpage.  */
    /* ??? When we do more than emulate Unix PALcode, we'll need to
       determine which KSEG is actually active.  */
    if (saddr < 0 && ((saddr >> 41) & 3) == 2) {
        /* User-space cannot access KSEG addresses.  */
        if (mmu_idx != MMU_KERNEL_IDX) {
            goto exit;
        }

        /* For the benefit of the Typhoon chipset, move bit 40 to bit 43.
           We would not do this if the 48-bit KSEG is enabled.  */
        phys = saddr & ((1ull << 40) - 1);
        phys |= (saddr & (1ull << 40)) << 3;

        prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
        ret = -1;
        goto exit;
    }

    /* Interpret the page table exactly like PALcode does.  */

    pt = env->ptbr;

    /* L1 page table read.  */
    index = (addr >> (TARGET_PAGE_BITS + 20)) & 0x3ff;
    L1pte = ldq_phys(pt + index*8);

    if (unlikely((L1pte & PTE_VALID) == 0)) {
        ret = MM_K_TNV;
        goto exit;
    }
    if (unlikely((L1pte & PTE_KRE) == 0)) {
        goto exit;
    }
    pt = L1pte >> 32 << TARGET_PAGE_BITS;

    /* L2 page table read.  */
    index = (addr >> (TARGET_PAGE_BITS + 10)) & 0x3ff;
    L2pte = ldq_phys(pt + index*8);

    if (unlikely((L2pte & PTE_VALID) == 0)) {
        ret = MM_K_TNV;
        goto exit;
    }
    if (unlikely((L2pte & PTE_KRE) == 0)) {
        goto exit;
    }
    pt = L2pte >> 32 << TARGET_PAGE_BITS;

    /* L3 page table read.  */
    index = (addr >> TARGET_PAGE_BITS) & 0x3ff;
    L3pte = ldq_phys(pt + index*8);

    phys = L3pte >> 32 << TARGET_PAGE_BITS;
    if (unlikely((L3pte & PTE_VALID) == 0)) {
        ret = MM_K_TNV;
        goto exit;
    }

#if PAGE_READ != 1 || PAGE_WRITE != 2 || PAGE_EXEC != 4
# error page bits out of date
#endif

    /* Check access violations.  */
    if (L3pte & (PTE_KRE << mmu_idx)) {
        prot |= PAGE_READ | PAGE_EXEC;
    }
    if (L3pte & (PTE_KWE << mmu_idx)) {
        prot |= PAGE_WRITE;
    }
    if (unlikely((prot & prot_need) == 0 && prot_need)) {
        goto exit;
    }

    /* Check fault-on-operation violations.  */
    prot &= ~(L3pte >> 1);
    ret = -1;
    if (unlikely((prot & prot_need) == 0)) {
        ret = (prot_need & PAGE_EXEC ? MM_K_FOE :
               prot_need & PAGE_WRITE ? MM_K_FOW :
               prot_need & PAGE_READ ? MM_K_FOR : -1);
    }

 exit:
    *pphys = phys;
    *pprot = prot;
    return ret;
}

hwaddr cpu_get_phys_page_debug(CPUTileraState *env, target_ulong addr)
{
    target_ulong phys;
    int prot, fail;

    fail = get_physical_address(env, addr, 0, 0, &phys, &prot);
    return (fail >= 0 ? -1 : phys);
}

int cpu_tilera_handle_mmu_fault(CPUTileraState *env, target_ulong addr, int rw,
                               int mmu_idx)
{
    target_ulong phys;
    int prot, fail;

    fail = get_physical_address(env, addr, 1 << rw, mmu_idx, &phys, &prot);
    if (unlikely(fail >= 0)) {
        env->exception_index = EXCP_MMFAULT;
        env->trap_arg0 = addr;
        env->trap_arg1 = fail;
        env->trap_arg2 = (rw == 2 ? -1 : rw);
        return 1;
    }

    tlb_set_page(env, addr & TARGET_PAGE_MASK, phys & TARGET_PAGE_MASK,
                 prot, mmu_idx, TARGET_PAGE_SIZE);
    return 0;
}
#endif /* USER_ONLY */

void do_interrupt (CPUTileraState *env)
{
    env->exception_index = -1;
	env->error_code = 0;
}

void cpu_dump_state (CPUTileraState *env, FILE *f, fprintf_function cpu_fprintf,
                     int flags)
{
    
    int i;
	
    for (i = 0; i < 52; i++) {
        cpu_fprintf(f, "gpr[%02d] " TARGET_FMT_lx " ", i, env->gpr[i]);
        if ((i % 3) == 0)
            cpu_fprintf(f, "\n");
    }

	cpu_fprintf(f, "      FP  " TARGET_FMT_lx "      TP  " TARGET_FMT_lx "\n", 
					env->gpr[TILE_REG_FP], env->gpr[TILE_REG_TP]);

	cpu_fprintf(f, "      SP  " TARGET_FMT_lx "      LR  " TARGET_FMT_lx "\n", 
					env->gpr[TILE_REG_SP], env->gpr[TILE_REG_LR]);

	cpu_fprintf(f, "      SN  " TARGET_FMT_lx "      PC  " TARGET_FMT_lx "\n", 
					env->sn, env->pc);

 	cpu_fprintf(f, "    IDN0  " TARGET_FMT_lx "    IDN1  " TARGET_FMT_lx "\n", 
					env->idn0, env->idn1);

	cpu_fprintf(f, "    UDN0  " TARGET_FMT_lx "    UDN1  " TARGET_FMT_lx "\n", 
					env->udn0, env->udn1);	

	cpu_fprintf(f, "    UDN2  " TARGET_FMT_lx "    UDN3  " TARGET_FMT_lx "\n", 
					env->udn2, env->udn3);

	
    cpu_fprintf(f, "\n");
}

void do_restore_state(CPUTileraState *env, uintptr_t retaddr)
{
    if (retaddr) {
        TranslationBlock *tb = tb_find_pc(retaddr);
        if (tb) {
            cpu_restore_state(tb, env, retaddr);
        }
    }
}

/* This should only be called from translate, via gen_excp.
   We expect that env->pc has already been updated.  */
void QEMU_NORETURN helper_excp(CPUTileraState *env, int excp, int error)
{
    env->exception_index = excp;
    env->error_code = error;
    cpu_loop_exit(env);
}

/* This may be called from any of the helpers to set up EXCEPTION_INDEX.  */
void QEMU_NORETURN dynamic_excp(CPUTileraState *env, uintptr_t retaddr,
                                int excp, int error)
{
    env->exception_index = excp;
    env->error_code = error;
    do_restore_state(env, retaddr);
    cpu_loop_exit(env);
}

void QEMU_NORETURN arith_excp(CPUTileraState *env, uintptr_t retaddr,
                              int exc, uint64_t mask)
{
    env->trap_arg0 = exc;
    env->trap_arg1 = mask;
    dynamic_excp(env, retaddr, EXCP_ARITH, 0);
}

