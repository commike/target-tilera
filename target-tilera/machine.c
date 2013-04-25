#include "hw/hw.h"
#include "hw/boards.h"

static VMStateField vmstate_cpu_fields[] = {
    VMSTATE_UINTTL_ARRAY(gpr, CPUTileraState, 56),
    VMSTATE_UINTTL(sn, CPUTileraState),
    VMSTATE_UINTTL(idn0, CPUTileraState),
    VMSTATE_UINTTL(idn1, CPUTileraState),
    VMSTATE_UINTTL(udn0, CPUTileraState),
    VMSTATE_UINTTL(udn1, CPUTileraState),
    VMSTATE_UINTTL(udn2, CPUTileraState),
    VMSTATE_UINTTL(udn3, CPUTileraState),
    VMSTATE_UINTTL(pc, CPUTileraState),
	VMSTATE_UINTTL_ARRAY(spr, CPUTileraState, SPR_MAX ),
    VMSTATE_END_OF_LIST()
};

static const VMStateDescription vmstate_cpu = {
    .name = "cpu",
    .version_id = 1,
    .minimum_version_id = 1,
    .minimum_version_id_old = 1,
    .fields = vmstate_cpu_fields,
};

void cpu_save(QEMUFile *f, void *opaque)
{
    vmstate_save_state(f, &vmstate_cpu, opaque);
}

int cpu_load(QEMUFile *f, void *opaque, int version_id)
{
    return vmstate_load_state(f, &vmstate_cpu, opaque, version_id);
}
