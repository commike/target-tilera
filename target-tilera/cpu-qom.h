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
#ifndef QEMU_TILERA_CPU_QOM_H
#define QEMU_TILERA_CPU_QOM_H

#include "qemu/cpu.h"
#include "cpu.h"

#define TYPE_TILERA_CPU "tilera-cpu"

#define TILERA_CPU_CLASS(klass) \
    OBJECT_CLASS_CHECK(TileraCPUClass, (klass), TYPE_TILERA_CPU)
#define TILERA_CPU(obj) \
    OBJECT_CHECK(TileraCPU, (obj), TYPE_TILERA_CPU)
#define TILERA_CPU_GET_CLASS(obj) \
    OBJECT_GET_CLASS(TileraCPUClass, (obj), TYPE_TILERA_CPU)

/**
 * TileraCPUClass:
 * @parent_reset: The parent class' reset handler.
 *
 * An Tilera CPU model.
 */
typedef struct TileraCPUClass {
    /*< private >*/
    CPUClass parent_class;
    /*< public >*/

    void (*parent_reset)(CPUState *cpu);
} TileraCPUClass;

/**
 * TileraCPU:
 * @env: #CPUTileraState
 *
 * An Tilera CPU.
 */
typedef struct TileraCPU {
    /*< private >*/
    CPUState parent_obj;
    /*< public >*/

    CPUTileraState env;
} TileraCPU;

static inline TileraCPU *tilera_env_get_cpu(CPUTileraState *env)
{
    return TILERA_CPU(container_of(env, TileraCPU, env));
}

#define ENV_GET_CPU(e) CPU(tilera_env_get_cpu(e))
/* hw/tilera_pic.c */
void cpu_tilera_pic_init(TileraCPU *cpu);

#endif
