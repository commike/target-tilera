/* THIS FILE IS AUTOMATICALLY GENERATED, DO NOT MODIFY */

/*
 * schema-defined QAPI visitor function
 *
 * Copyright IBM, Corp. 2011
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.1 or later.
 * See the COPYING.LIB file in the top-level directory.
 *
 */

#ifndef QAPI_VISIT_H
#define QAPI_VISIT_H

#include "qapi/qapi-visit-core.h"
#include "qapi-types.h"

void visit_type_ErrorClass(Visitor *m, ErrorClass * obj, const char *name, Error **errp);
void visit_type_ErrorClassList(Visitor *m, ErrorClassList ** obj, const char *name, Error **errp);

void visit_type_NameInfo(Visitor *m, NameInfo ** obj, const char *name, Error **errp);
void visit_type_NameInfoList(Visitor *m, NameInfoList ** obj, const char *name, Error **errp);

void visit_type_VersionInfo(Visitor *m, VersionInfo ** obj, const char *name, Error **errp);
void visit_type_VersionInfoList(Visitor *m, VersionInfoList ** obj, const char *name, Error **errp);

void visit_type_KvmInfo(Visitor *m, KvmInfo ** obj, const char *name, Error **errp);
void visit_type_KvmInfoList(Visitor *m, KvmInfoList ** obj, const char *name, Error **errp);

void visit_type_RunState(Visitor *m, RunState * obj, const char *name, Error **errp);
void visit_type_RunStateList(Visitor *m, RunStateList ** obj, const char *name, Error **errp);

void visit_type_SnapshotInfo(Visitor *m, SnapshotInfo ** obj, const char *name, Error **errp);
void visit_type_SnapshotInfoList(Visitor *m, SnapshotInfoList ** obj, const char *name, Error **errp);

void visit_type_ImageInfo(Visitor *m, ImageInfo ** obj, const char *name, Error **errp);
void visit_type_ImageInfoList(Visitor *m, ImageInfoList ** obj, const char *name, Error **errp);

void visit_type_StatusInfo(Visitor *m, StatusInfo ** obj, const char *name, Error **errp);
void visit_type_StatusInfoList(Visitor *m, StatusInfoList ** obj, const char *name, Error **errp);

void visit_type_UuidInfo(Visitor *m, UuidInfo ** obj, const char *name, Error **errp);
void visit_type_UuidInfoList(Visitor *m, UuidInfoList ** obj, const char *name, Error **errp);

void visit_type_ChardevInfo(Visitor *m, ChardevInfo ** obj, const char *name, Error **errp);
void visit_type_ChardevInfoList(Visitor *m, ChardevInfoList ** obj, const char *name, Error **errp);

void visit_type_CommandInfo(Visitor *m, CommandInfo ** obj, const char *name, Error **errp);
void visit_type_CommandInfoList(Visitor *m, CommandInfoList ** obj, const char *name, Error **errp);

void visit_type_EventInfo(Visitor *m, EventInfo ** obj, const char *name, Error **errp);
void visit_type_EventInfoList(Visitor *m, EventInfoList ** obj, const char *name, Error **errp);

void visit_type_MigrationStats(Visitor *m, MigrationStats ** obj, const char *name, Error **errp);
void visit_type_MigrationStatsList(Visitor *m, MigrationStatsList ** obj, const char *name, Error **errp);

void visit_type_XBZRLECacheStats(Visitor *m, XBZRLECacheStats ** obj, const char *name, Error **errp);
void visit_type_XBZRLECacheStatsList(Visitor *m, XBZRLECacheStatsList ** obj, const char *name, Error **errp);

void visit_type_MigrationInfo(Visitor *m, MigrationInfo ** obj, const char *name, Error **errp);
void visit_type_MigrationInfoList(Visitor *m, MigrationInfoList ** obj, const char *name, Error **errp);

void visit_type_MigrationCapability(Visitor *m, MigrationCapability * obj, const char *name, Error **errp);
void visit_type_MigrationCapabilityList(Visitor *m, MigrationCapabilityList ** obj, const char *name, Error **errp);

void visit_type_MigrationCapabilityStatus(Visitor *m, MigrationCapabilityStatus ** obj, const char *name, Error **errp);
void visit_type_MigrationCapabilityStatusList(Visitor *m, MigrationCapabilityStatusList ** obj, const char *name, Error **errp);

void visit_type_MouseInfo(Visitor *m, MouseInfo ** obj, const char *name, Error **errp);
void visit_type_MouseInfoList(Visitor *m, MouseInfoList ** obj, const char *name, Error **errp);

void visit_type_CpuInfo(Visitor *m, CpuInfo ** obj, const char *name, Error **errp);
void visit_type_CpuInfoList(Visitor *m, CpuInfoList ** obj, const char *name, Error **errp);

void visit_type_BlockDeviceInfo(Visitor *m, BlockDeviceInfo ** obj, const char *name, Error **errp);
void visit_type_BlockDeviceInfoList(Visitor *m, BlockDeviceInfoList ** obj, const char *name, Error **errp);

void visit_type_BlockDeviceIoStatus(Visitor *m, BlockDeviceIoStatus * obj, const char *name, Error **errp);
void visit_type_BlockDeviceIoStatusList(Visitor *m, BlockDeviceIoStatusList ** obj, const char *name, Error **errp);

void visit_type_BlockDirtyInfo(Visitor *m, BlockDirtyInfo ** obj, const char *name, Error **errp);
void visit_type_BlockDirtyInfoList(Visitor *m, BlockDirtyInfoList ** obj, const char *name, Error **errp);

void visit_type_BlockInfo(Visitor *m, BlockInfo ** obj, const char *name, Error **errp);
void visit_type_BlockInfoList(Visitor *m, BlockInfoList ** obj, const char *name, Error **errp);

void visit_type_BlockDeviceStats(Visitor *m, BlockDeviceStats ** obj, const char *name, Error **errp);
void visit_type_BlockDeviceStatsList(Visitor *m, BlockDeviceStatsList ** obj, const char *name, Error **errp);

void visit_type_BlockStats(Visitor *m, BlockStats ** obj, const char *name, Error **errp);
void visit_type_BlockStatsList(Visitor *m, BlockStatsList ** obj, const char *name, Error **errp);

void visit_type_VncClientInfo(Visitor *m, VncClientInfo ** obj, const char *name, Error **errp);
void visit_type_VncClientInfoList(Visitor *m, VncClientInfoList ** obj, const char *name, Error **errp);

void visit_type_VncInfo(Visitor *m, VncInfo ** obj, const char *name, Error **errp);
void visit_type_VncInfoList(Visitor *m, VncInfoList ** obj, const char *name, Error **errp);

void visit_type_SpiceChannel(Visitor *m, SpiceChannel ** obj, const char *name, Error **errp);
void visit_type_SpiceChannelList(Visitor *m, SpiceChannelList ** obj, const char *name, Error **errp);

void visit_type_SpiceQueryMouseMode(Visitor *m, SpiceQueryMouseMode * obj, const char *name, Error **errp);
void visit_type_SpiceQueryMouseModeList(Visitor *m, SpiceQueryMouseModeList ** obj, const char *name, Error **errp);

void visit_type_SpiceInfo(Visitor *m, SpiceInfo ** obj, const char *name, Error **errp);
void visit_type_SpiceInfoList(Visitor *m, SpiceInfoList ** obj, const char *name, Error **errp);

void visit_type_BalloonInfo(Visitor *m, BalloonInfo ** obj, const char *name, Error **errp);
void visit_type_BalloonInfoList(Visitor *m, BalloonInfoList ** obj, const char *name, Error **errp);

void visit_type_PciMemoryRange(Visitor *m, PciMemoryRange ** obj, const char *name, Error **errp);
void visit_type_PciMemoryRangeList(Visitor *m, PciMemoryRangeList ** obj, const char *name, Error **errp);

void visit_type_PciMemoryRegion(Visitor *m, PciMemoryRegion ** obj, const char *name, Error **errp);
void visit_type_PciMemoryRegionList(Visitor *m, PciMemoryRegionList ** obj, const char *name, Error **errp);

void visit_type_PciBridgeInfo(Visitor *m, PciBridgeInfo ** obj, const char *name, Error **errp);
void visit_type_PciBridgeInfoList(Visitor *m, PciBridgeInfoList ** obj, const char *name, Error **errp);

void visit_type_PciDeviceInfo(Visitor *m, PciDeviceInfo ** obj, const char *name, Error **errp);
void visit_type_PciDeviceInfoList(Visitor *m, PciDeviceInfoList ** obj, const char *name, Error **errp);

void visit_type_PciInfo(Visitor *m, PciInfo ** obj, const char *name, Error **errp);
void visit_type_PciInfoList(Visitor *m, PciInfoList ** obj, const char *name, Error **errp);

void visit_type_BlockdevOnError(Visitor *m, BlockdevOnError * obj, const char *name, Error **errp);
void visit_type_BlockdevOnErrorList(Visitor *m, BlockdevOnErrorList ** obj, const char *name, Error **errp);

void visit_type_MirrorSyncMode(Visitor *m, MirrorSyncMode * obj, const char *name, Error **errp);
void visit_type_MirrorSyncModeList(Visitor *m, MirrorSyncModeList ** obj, const char *name, Error **errp);

void visit_type_BlockJobInfo(Visitor *m, BlockJobInfo ** obj, const char *name, Error **errp);
void visit_type_BlockJobInfoList(Visitor *m, BlockJobInfoList ** obj, const char *name, Error **errp);

void visit_type_NewImageMode(Visitor *m, NewImageMode * obj, const char *name, Error **errp);
void visit_type_NewImageModeList(Visitor *m, NewImageModeList ** obj, const char *name, Error **errp);

void visit_type_BlockdevSnapshot(Visitor *m, BlockdevSnapshot ** obj, const char *name, Error **errp);
void visit_type_BlockdevSnapshotList(Visitor *m, BlockdevSnapshotList ** obj, const char *name, Error **errp);

void visit_type_BlockdevActionKind(Visitor *m, BlockdevActionKind * obj, const char *name, Error **errp);

void visit_type_BlockdevAction(Visitor *m, BlockdevAction ** obj, const char *name, Error **errp);
void visit_type_BlockdevActionList(Visitor *m, BlockdevActionList ** obj, const char *name, Error **errp);

void visit_type_ObjectPropertyInfo(Visitor *m, ObjectPropertyInfo ** obj, const char *name, Error **errp);
void visit_type_ObjectPropertyInfoList(Visitor *m, ObjectPropertyInfoList ** obj, const char *name, Error **errp);

void visit_type_ObjectTypeInfo(Visitor *m, ObjectTypeInfo ** obj, const char *name, Error **errp);
void visit_type_ObjectTypeInfoList(Visitor *m, ObjectTypeInfoList ** obj, const char *name, Error **errp);

void visit_type_DevicePropertyInfo(Visitor *m, DevicePropertyInfo ** obj, const char *name, Error **errp);
void visit_type_DevicePropertyInfoList(Visitor *m, DevicePropertyInfoList ** obj, const char *name, Error **errp);

void visit_type_NetdevNoneOptions(Visitor *m, NetdevNoneOptions ** obj, const char *name, Error **errp);
void visit_type_NetdevNoneOptionsList(Visitor *m, NetdevNoneOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetLegacyNicOptions(Visitor *m, NetLegacyNicOptions ** obj, const char *name, Error **errp);
void visit_type_NetLegacyNicOptionsList(Visitor *m, NetLegacyNicOptionsList ** obj, const char *name, Error **errp);

void visit_type_String(Visitor *m, String ** obj, const char *name, Error **errp);
void visit_type_StringList(Visitor *m, StringList ** obj, const char *name, Error **errp);

void visit_type_NetdevUserOptions(Visitor *m, NetdevUserOptions ** obj, const char *name, Error **errp);
void visit_type_NetdevUserOptionsList(Visitor *m, NetdevUserOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetdevTapOptions(Visitor *m, NetdevTapOptions ** obj, const char *name, Error **errp);
void visit_type_NetdevTapOptionsList(Visitor *m, NetdevTapOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetdevSocketOptions(Visitor *m, NetdevSocketOptions ** obj, const char *name, Error **errp);
void visit_type_NetdevSocketOptionsList(Visitor *m, NetdevSocketOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetdevVdeOptions(Visitor *m, NetdevVdeOptions ** obj, const char *name, Error **errp);
void visit_type_NetdevVdeOptionsList(Visitor *m, NetdevVdeOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetdevDumpOptions(Visitor *m, NetdevDumpOptions ** obj, const char *name, Error **errp);
void visit_type_NetdevDumpOptionsList(Visitor *m, NetdevDumpOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetdevBridgeOptions(Visitor *m, NetdevBridgeOptions ** obj, const char *name, Error **errp);
void visit_type_NetdevBridgeOptionsList(Visitor *m, NetdevBridgeOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetdevHubPortOptions(Visitor *m, NetdevHubPortOptions ** obj, const char *name, Error **errp);
void visit_type_NetdevHubPortOptionsList(Visitor *m, NetdevHubPortOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetClientOptionsKind(Visitor *m, NetClientOptionsKind * obj, const char *name, Error **errp);

void visit_type_NetClientOptions(Visitor *m, NetClientOptions ** obj, const char *name, Error **errp);
void visit_type_NetClientOptionsList(Visitor *m, NetClientOptionsList ** obj, const char *name, Error **errp);

void visit_type_NetLegacy(Visitor *m, NetLegacy ** obj, const char *name, Error **errp);
void visit_type_NetLegacyList(Visitor *m, NetLegacyList ** obj, const char *name, Error **errp);

void visit_type_Netdev(Visitor *m, Netdev ** obj, const char *name, Error **errp);
void visit_type_NetdevList(Visitor *m, NetdevList ** obj, const char *name, Error **errp);

void visit_type_InetSocketAddress(Visitor *m, InetSocketAddress ** obj, const char *name, Error **errp);
void visit_type_InetSocketAddressList(Visitor *m, InetSocketAddressList ** obj, const char *name, Error **errp);

void visit_type_UnixSocketAddress(Visitor *m, UnixSocketAddress ** obj, const char *name, Error **errp);
void visit_type_UnixSocketAddressList(Visitor *m, UnixSocketAddressList ** obj, const char *name, Error **errp);

void visit_type_SocketAddressKind(Visitor *m, SocketAddressKind * obj, const char *name, Error **errp);

void visit_type_SocketAddress(Visitor *m, SocketAddress ** obj, const char *name, Error **errp);
void visit_type_SocketAddressList(Visitor *m, SocketAddressList ** obj, const char *name, Error **errp);

void visit_type_MachineInfo(Visitor *m, MachineInfo ** obj, const char *name, Error **errp);
void visit_type_MachineInfoList(Visitor *m, MachineInfoList ** obj, const char *name, Error **errp);

void visit_type_CpuDefinitionInfo(Visitor *m, CpuDefinitionInfo ** obj, const char *name, Error **errp);
void visit_type_CpuDefinitionInfoList(Visitor *m, CpuDefinitionInfoList ** obj, const char *name, Error **errp);

void visit_type_AddfdInfo(Visitor *m, AddfdInfo ** obj, const char *name, Error **errp);
void visit_type_AddfdInfoList(Visitor *m, AddfdInfoList ** obj, const char *name, Error **errp);

void visit_type_FdsetFdInfo(Visitor *m, FdsetFdInfo ** obj, const char *name, Error **errp);
void visit_type_FdsetFdInfoList(Visitor *m, FdsetFdInfoList ** obj, const char *name, Error **errp);

void visit_type_FdsetInfo(Visitor *m, FdsetInfo ** obj, const char *name, Error **errp);
void visit_type_FdsetInfoList(Visitor *m, FdsetInfoList ** obj, const char *name, Error **errp);

void visit_type_TargetType(Visitor *m, TargetType * obj, const char *name, Error **errp);
void visit_type_TargetTypeList(Visitor *m, TargetTypeList ** obj, const char *name, Error **errp);

void visit_type_TargetInfo(Visitor *m, TargetInfo ** obj, const char *name, Error **errp);
void visit_type_TargetInfoList(Visitor *m, TargetInfoList ** obj, const char *name, Error **errp);

void visit_type_QKeyCode(Visitor *m, QKeyCode * obj, const char *name, Error **errp);
void visit_type_QKeyCodeList(Visitor *m, QKeyCodeList ** obj, const char *name, Error **errp);

void visit_type_KeyValueKind(Visitor *m, KeyValueKind * obj, const char *name, Error **errp);

void visit_type_KeyValue(Visitor *m, KeyValue ** obj, const char *name, Error **errp);
void visit_type_KeyValueList(Visitor *m, KeyValueList ** obj, const char *name, Error **errp);

#endif
