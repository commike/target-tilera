/*
 * QEMU Tilera CPU
 *
 * Copyright (c) 2013 Sarmad Tanwir
 * Copyright (c) 2013 System Software Research Group, ECE, Virginia Tech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <http://www.gnu.org/licenses/lgpl-2.1.html>
 */

#include "cpu.h"
#include "qemu-common.h"


static void tilera_cpu_initfn(Object *obj)
{
    TileraCPU *cpu = TILERA_CPU(obj);
    CPUTileraState *env = &cpu->env;

    cpu_exec_init(env);
    tlb_flush(env, 1);

}

static const TypeInfo tilera_cpu_type_info = {
    .name = TYPE_TILERA_CPU,
    .parent = TYPE_CPU,
    .instance_size = sizeof(TileraCPU),
    .instance_init = tilera_cpu_initfn,
    .abstract = false,
    .class_size = sizeof(TileraCPUClass),
};

static void tilera_cpu_register_types(void)
{
    type_register_static(&tilera_cpu_type_info);
}

type_init(tilera_cpu_register_types)
