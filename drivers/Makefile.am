# Network UPS Tools: drivers

# Export certain values for ccache which NUT ci_build.sh can customize,
# to facilitate developer iteration re-runs of "make" later.
# At least GNU and BSD make implementations are okay with this syntax.
@NUT_AM_MAKE_CAN_EXPORT@@NUT_AM_EXPORT_CCACHE_NAMESPACE@export CCACHE_NAMESPACE=@CCACHE_NAMESPACE@
@NUT_AM_MAKE_CAN_EXPORT@@NUT_AM_EXPORT_CCACHE_BASEDIR@export CCACHE_BASEDIR=@CCACHE_BASEDIR@
@NUT_AM_MAKE_CAN_EXPORT@@NUT_AM_EXPORT_CCACHE_DIR@export CCACHE_DIR=@CCACHE_DIR@
@NUT_AM_MAKE_CAN_EXPORT@@NUT_AM_EXPORT_CCACHE_PATH@export CCACHE_PATH=@CCACHE_PATH@
@NUT_AM_MAKE_CAN_EXPORT@@NUT_AM_EXPORT_CCACHE_PATH@export PATH=@PATH_DURING_CONFIGURE@

# Make sure out-of-dir dependencies exist (especially when dev-building parts):
$(top_builddir)/common/libcommon.la \
$(top_builddir)/common/libcommonversion.la \
$(top_builddir)/common/libparseconf.la \
$(top_builddir)/clients/libupsclient.la: dummy
	+@cd $(@D) && $(MAKE) $(AM_MAKEFLAGS) $(@F)

# by default, link programs in this directory with libcommon.la
# (libtool version of the static lib, in order to access LTLIBOBJS)
#FIXME: SERLIBS is only useful for LDADD_DRIVERS_SERIAL not for LDADD_COMMON
LDADD_COMMON = \
	$(top_builddir)/common/libcommon.la \
	$(top_builddir)/common/libcommonversion.la \
	$(top_builddir)/common/libparseconf.la
LDADD_DRIVERS = libdummy.la libdummy_upsdrvquery.la $(LDADD_COMMON)
LDADD_DRIVERS_SERIAL = libdummy_serial.la $(LDADD_DRIVERS) $(SERLIBS)

# most targets are serial drivers, so make this the default
LDADD = $(LDADD_DRIVERS_SERIAL)

# Avoid per-target CFLAGS, because this will prevent re-use of object
# files. In any case, CFLAGS are only -I options, so there is no harm,
# but only add them if we really use the target.
AM_CFLAGS = -I$(top_srcdir)/include
if WITH_USB
  AM_CFLAGS += $(LIBUSB_CFLAGS)
endif WITH_USB
if WITH_NEON
  AM_CFLAGS += $(LIBNEON_CFLAGS)
endif WITH_NEON
if WITH_LIBPOWERMAN
  AM_CFLAGS += $(LIBPOWERMAN_CFLAGS)
endif WITH_LIBPOWERMAN
if WITH_IPMI
  AM_CFLAGS += $(LIBIPMI_CFLAGS)
endif WITH_IPMI
if WITH_GPIO
  AM_CFLAGS += $(LIBGPIO_CFLAGS)
endif WITH_GPIO
if WITH_MODBUS
  AM_CFLAGS += $(LIBMODBUS_CFLAGS)
endif WITH_MODBUS
if HAVE_LIBREGEX
  AM_CFLAGS += $(LIBREGEX_CFLAGS)
endif HAVE_LIBREGEX

# Allow top-level Makefile override NUTSW_DRIVERLIST_DUMMY_UPS="dummy" to run
# a build scenario without this driver (it is in a different dependency)
# NOTE: Must be defaulted with EXEEXT to avoid rewrites by automake parser
# which defeat the purpose of it being optionally built via `make all/drivers`
# in top level.
NUTSW_DRIVERLIST_DUMMY_UPS = dummy-ups$(EXEEXT)
NUTSW_DRIVERLIST = $(NUTSW_DRIVERLIST_DUMMY_UPS) \
 clone clone-outlet failover apcupsd-ups skel
SERIAL_DRIVERLIST = al175 bcmxcp belkin belkinunv bestfcom	\
 bestfortress bestuferrups bestups etapro everups 	\
 gamatronic genericups isbmex liebert liebert-esp2 liebert-gxe masterguard metasys	\
 mge-utalk microdowell microsol-apc mge-shut nutdrv_hashx oneac optiups powercom powervar_cx_ser rhino	\
 safenet nutdrv_siemens-sitop solis tripplite tripplitesu upscode2 victronups powerpanel \
 blazer_ser ivtscd apcsmart apcsmart-old riello_ser sms_ser bicker_ser ve-direct
if HAVE_LINUX_SERIAL_H
# Temporary, until ported to more OSes
SERIAL_DRIVERLIST += nhs_ser
endif HAVE_LINUX_SERIAL_H
SNMP_DRIVERLIST = snmp-ups
USB_LIBUSB_DRIVERLIST = usbhid-ups bcmxcp_usb tripplite_usb \
 blazer_usb richcomm_usb riello_usb powervar_cx_usb \
 nutdrv_atcl_usb
USB_DRIVERLIST = $(USB_LIBUSB_DRIVERLIST)
SERIAL_USB_DRIVERLIST = \
	nutdrv_qx
NEONXML_DRIVERLIST = netxml-ups
MACOSX_DRIVERLIST = macosx-ups
MODBUS_DRIVERLIST = phoenixcontact_modbus generic_modbus huawei-ups2000 socomec_jbus adelsystem_cbi apc_modbus
LINUX_I2C_DRIVERLIST = asem pijuice hwmon_ina219
POWERMAN_DRIVERLIST = powerman-pdu
IPMI_DRIVERLIST = nut-ipmipsu
GPIO_DRIVERLIST = generic_gpio_libgpiod

# distribute all drivers, even ones that are not built by default
EXTRA_PROGRAMS  = $(SERIAL_DRIVERLIST) $(USB_DRIVERLIST) $(SERIAL_USB_DRIVERLIST)
EXTRA_PROGRAMS += $(SNMP_DRIVERLIST) $(NEONXML_DRIVERLIST) $(MACOSX_DRIVERLIST)
EXTRA_PROGRAMS += $(LINUX_I2C_DRIVERLIST)
EXTRA_PROGRAMS += $(NUTSW_DRIVERLIST)
EXTRA_PROGRAMS += $(GPIO_DRIVERLIST)

# construct the list of drivers to build
if SOME_DRIVERS
 driverexec_PROGRAMS = $(DRIVER_BUILD_LIST)
else !SOME_DRIVERS
 driverexec_PROGRAMS = $(NUTSW_DRIVERLIST)
if WITH_SERIAL
  driverexec_PROGRAMS += $(SERIAL_DRIVERLIST) $(SERIAL_USB_DRIVERLIST)
else !WITH_SERIAL
if WITH_USB
  driverexec_PROGRAMS += $(SERIAL_USB_DRIVERLIST)
endif WITH_USB
endif !WITH_SERIAL
if WITH_SNMP
  driverexec_PROGRAMS += $(SNMP_DRIVERLIST)
endif WITH_SNMP
if WITH_USB
   driverexec_PROGRAMS += $(USB_LIBUSB_DRIVERLIST)
endif WITH_USB
if WITH_NEON
   driverexec_PROGRAMS += $(NEONXML_DRIVERLIST)
endif WITH_NEON
if WITH_LIBPOWERMAN
   driverexec_PROGRAMS += $(POWERMAN_DRIVERLIST)
endif WITH_LIBPOWERMAN
if WITH_IPMI
   driverexec_PROGRAMS += $(IPMI_DRIVERLIST)
endif WITH_IPMI
if WITH_GPIO
   driverexec_PROGRAMS += $(GPIO_DRIVERLIST)
endif WITH_GPIO
if WITH_MACOSX
   driverexec_PROGRAMS += $(MACOSX_DRIVERLIST)
endif WITH_MACOSX
if WITH_LINUX_I2C
   driverexec_PROGRAMS += $(LINUX_I2C_DRIVERLIST)
endif WITH_LINUX_I2C
if WITH_MODBUS
  driverexec_PROGRAMS += $(MODBUS_DRIVERLIST)
endif WITH_MODBUS
endif !SOME_DRIVERS

# always build upsdrvctl
sbin_PROGRAMS = upsdrvctl

# ==========================================================================
# Driver build details

# upsdrvctl: the all-singing all-dancing driver control program
upsdrvctl_SOURCES = upsdrvctl.c
upsdrvctl_LDADD = $(LDADD_COMMON) libdummy_upsdrvquery.la

# serial drivers: all of them use standard LDADD and CFLAGS
al175_SOURCES = al175.c
apcsmart_SOURCES = apcsmart.c apcsmart_tabs.c
apcsmart_LDADD = $(LDADD) $(LIBREGEX_LIBS)
apcsmart_old_SOURCES = apcsmart-old.c
bcmxcp_SOURCES = bcmxcp.c bcmxcp_ser.c
bcmxcp_LDADD = $(LDADD) -lm
belkin_SOURCES = belkin.c
belkinunv_SOURCES = belkinunv.c
bestfcom_SOURCES = bestfcom.c
bestfortress_SOURCES = bestfortress.c
bestuferrups_SOURCES = bestuferrups.c
bestups_SOURCES = bestups.c
blazer_ser_SOURCES = blazer.c blazer_ser.c
blazer_ser_LDADD = $(LDADD) -lm
etapro_SOURCES = etapro.c
everups_SOURCES = everups.c
gamatronic_SOURCES = gamatronic.c
genericups_SOURCES = genericups.c
isbmex_SOURCES = isbmex.c
isbmex_LDADD = $(LDADD) -lm
ivtscd_SOURCES = ivtscd.c
liebert_SOURCES = liebert.c
liebert_esp2_SOURCES = liebert-esp2.c
liebert_gxe_SOURCES = liebert-gxe.c
masterguard_SOURCES = masterguard.c
metasys_SOURCES = metasys.c
metasys_LDADD = $(LDADD) -lm
mge_utalk_SOURCES = mge-utalk.c
microdowell_SOURCES = microdowell.c
microsol_apc_SOURCES = microsol-apc.c microsol-common.c
microsol_apc_LDADD = $(LDADD) -lm
nhs_ser_SOURCES = nhs_ser.c
nhs_ser_LDADD = $(LDADD) -lm
nutdrv_hashx_SOURCES = nutdrv_hashx.c
nutdrv_hashx_LDADD = $(LDADD) -lm
oneac_SOURCES = oneac.c
optiups_SOURCES = optiups.c
powercom_SOURCES = powercom.c
powercom_LDADD = $(LDADD) -lm
powerpanel_SOURCES = powerpanel.c powerp-bin.c powerp-txt.c
powerpanel_LDADD = $(LDADD) -lm
powervar_cx_ser_SOURCES = powervar_cx_ser.c powervar_cx.c
powervar_cx_ser_CFLAGS = $(AM_CFLAGS) -DPVAR_SERIAL=1
rhino_SOURCES = rhino.c
rhino_LDADD = $(LDADD) -lm
safenet_SOURCES = safenet.c
nutdrv_siemens_sitop_SOURCES = nutdrv_siemens_sitop.c
solis_SOURCES = solis.c
solis_LDADD = $(LDADD) -lm
tripplite_SOURCES = tripplite.c
tripplite_LDADD = $(LDADD) -lm
tripplitesu_SOURCES = tripplitesu.c
upscode2_SOURCES = upscode2.c
upscode2_LDADD = $(LDADD) -lm
victronups_SOURCES = victronups.c
riello_ser_SOURCES = riello.c riello_ser.c
riello_ser_LDADD = $(LDADD) -lm
sms_ser_SOURCES = sms_ser.c
sms_ser_LDADD = $(LDADD) -lm
bicker_ser_SOURCES = bicker_ser.c
bicker_ser_LDADD = $(LDADD) -lm
ve_direct_SOURCES = ve-direct.c
ve_direct_LDADD = $(LDADD) -lm

# non-serial drivers: these use custom LDADD and/or CFLAGS

# dummy (in NUTSW_DRIVERLIST)
dummy_ups_SOURCES = dummy-ups.c
dummy_ups_CFLAGS = $(AM_CFLAGS) -I$(top_srcdir)/clients
dummy_ups_LDADD = $(LDADD_DRIVERS) $(top_builddir)/clients/libupsclient.la
dummy_ups_LDFLAGS = $(AM_LDFLAGS)
if WITH_SSL
  dummy_ups_CFLAGS += $(LIBSSL_CFLAGS)
  dummy_ups_LDADD += $(LIBSSL_LIBS)
  dummy_ups_LDFLAGS += $(LIBSSL_LDFLAGS_RPATH)
endif WITH_SSL

# Clone drivers (in NUTSW_DRIVERLIST)
clone_SOURCES = clone.c
clone_outlet_SOURCES = clone-outlet.c

# failover driver (in NUTSW_DRIVERLIST)
failover_SOURCES = failover.c

# apcupsd client driver (in NUTSW_DRIVERLIST)
apcupsd_ups_SOURCES = apcupsd-ups.c
apcupsd_ups_CFLAGS = $(AM_CFLAGS)
apcupsd_ups_LDADD = $(LDADD_DRIVERS)

# sample skeleton driver (in NUTSW_DRIVERLIST)

skel_SOURCES = skel.c
skel_LDADD = $(LDADD_DRIVERS)

# USB
if WITH_LIBUSB_0_1
LIBUSB_IMPL = libusb0.c
endif WITH_LIBUSB_0_1
if WITH_LIBUSB_1_0
LIBUSB_IMPL = libusb1.c
endif WITH_LIBUSB_1_0
USBHID_UPS_SUBDRIVERS = apc-hid.c arduino-hid.c belkin-hid.c cps-hid.c explore-hid.c \
 liebert-hid.c mge-hid.c powercom-hid.c tripplite-hid.c idowell-hid.c \
 openups-hid.c powervar-hid.c delta_ups-hid.c ecoflow-hid.c ever-hid.c \
 legrand-hid.c salicru-hid.c
usbhid_ups_SOURCES = usbhid-ups.c libhid.c $(LIBUSB_IMPL) hidparser.c	\
 usb-common.c $(USBHID_UPS_SUBDRIVERS)
usbhid_ups_LDADD = $(LDADD_DRIVERS) $(LIBUSB_LIBS) -lm

powervar_cx_usb_SOURCES = powervar_cx_usb.c powervar_cx.c $(LIBUSB_IMPL) usb-common.c
powervar_cx_usb_CFLAGS = $(AM_CFLAGS) -DPVAR_USB=1
powervar_cx_usb_LDADD = $(LDADD_DRIVERS) $(LIBUSB_LIBS) -lm

tripplite_usb_SOURCES = tripplite_usb.c $(LIBUSB_IMPL) usb-common.c
tripplite_usb_LDADD = $(LDADD_DRIVERS) $(LIBUSB_LIBS) -lm

bcmxcp_usb_SOURCES = bcmxcp_usb.c bcmxcp.c usb-common.c
bcmxcp_usb_LDADD = $(LDADD_DRIVERS) $(LIBUSB_LIBS) -lm

blazer_usb_SOURCES = blazer.c blazer_usb.c $(LIBUSB_IMPL) usb-common.c
blazer_usb_LDADD = $(LDADD_DRIVERS) $(LIBUSB_LIBS) -lm

nutdrv_atcl_usb_SOURCES = nutdrv_atcl_usb.c usb-common.c
nutdrv_atcl_usb_LDADD = $(LDADD_DRIVERS) $(LIBUSB_LIBS)

richcomm_usb_SOURCES = richcomm_usb.c $(LIBUSB_IMPL) usb-common.c
richcomm_usb_LDADD = $(LDADD_DRIVERS) $(LIBUSB_LIBS)

riello_usb_SOURCES = riello.c riello_usb.c $(LIBUSB_IMPL) usb-common.c
riello_usb_LDADD = $(LDADD_DRIVERS) $(LIBUSB_LIBS) -lm

# HID-over-serial
mge_shut_SOURCES = usbhid-ups.c libshut.c libhid.c hidparser.c mge-hid.c
# per-target CFLAGS are necessary here
mge_shut_CFLAGS = $(AM_CFLAGS) -DSHUT_MODE=1
mge_shut_LDADD = $(LDADD) -lm

# SNMP
# Please keep the MIB table below sorted roughly alphabetically (incidentally
# by vendor too) to ease maintenance and codebase fork resynchronisations
snmp_ups_SOURCES = snmp-ups.c snmp-ups-helpers.c \
 apc-mib.c apc-pdu-mib.c apc-epdu-mib.c \
 baytech-mib.c baytech-rpc3nc-mib.c bestpower-mib.c \
 compaq-mib.c cyberpower-mib.c \
 delta_ups-mib.c \
 eaton-pdu-genesis2-mib.c eaton-pdu-marlin-mib.c eaton-pdu-marlin-helpers.c \
 eaton-pdu-pulizzi-mib.c eaton-pdu-revelation-mib.c eaton-pdu-nlogic-mib.c \
 eaton-ats16-nmc-mib.c eaton-ats16-nm2-mib.c apc-ats-mib.c eaton-ats30-mib.c \
 eaton-ups-pwnm2-mib.c eaton-ups-pxg-mib.c \
 emerson-avocent-pdu-mib.c \
 hpe-pdu-mib.c hpe-pdu3-cis-mib.c huawei-mib.c \
 ietf-mib.c \
 mge-mib.c \
 netvision-mib.c \
 raritan-pdu-mib.c raritan-px2-mib.c \
 xppc-mib.c
snmp_ups_CFLAGS = $(AM_CFLAGS)
snmp_ups_CFLAGS += $(LIBNETSNMP_CFLAGS)
snmp_ups_LDADD = $(LDADD_DRIVERS) $(LIBNETSNMP_LIBS) -lm
snmp_ups_LDFLAGS = $(AM_LDFLAGS)

if WITH_SSL
if !WITH_OPENSSL
  snmp_ups_CFLAGS += -UNETSNMP_USE_OPENSSL
endif !WITH_OPENSSL
  snmp_ups_CFLAGS += $(LIBSSL_CFLAGS)
  snmp_ups_LDADD += $(LIBSSL_LIBS)
  snmp_ups_LDFLAGS += $(LIBSSL_LDFLAGS_RPATH)
endif WITH_SSL

# NEON XML/HTTP
netxml_ups_SOURCES = netxml-ups.c mge-xml.c
netxml_ups_LDADD = $(LDADD_DRIVERS) $(LIBNEON_LIBS)
netxml_ups_CFLAGS = $(AM_CFLAGS)
netxml_ups_LDFLAGS = $(AM_LDFLAGS)

if WITH_SSL
  netxml_ups_CFLAGS += $(LIBSSL_CFLAGS)
  netxml_ups_LDADD += $(LIBSSL_LIBS)
  netxml_ups_LDFLAGS += $(LIBSSL_LDFLAGS_RPATH)
endif WITH_SSL

# Powerman
powerman_pdu_SOURCES = powerman-pdu.c
powerman_pdu_LDADD = $(LDADD) $(LIBPOWERMAN_LIBS)

# IPMI PSU
nut_ipmipsu_SOURCES = nut-ipmipsu.c
if WITH_FREEIPMI
  nut_ipmipsu_SOURCES += nut-libfreeipmi.c
endif WITH_FREEIPMI
# FIXME: Hacky hot-fix for build agents of varying OS generations:
# Different versions of IPMI libs requested 'unsigned int *' or 'int *' args:
#nut_ipmipsu_CFLAGS = $(AM_CFLAGS) -Wno-pointer-sign
nut_ipmipsu_LDADD = $(LDADD) $(LIBIPMI_LIBS)

# Mac OS X metadriver
macosx_ups_LDADD = $(LDADD_DRIVERS)
macosx_ups_LDFLAGS = $(LDFLAGS) -framework IOKit -framework CoreFoundation
macosx_ups_SOURCES = macosx-ups.c

# Modbus drivers
phoenixcontact_modbus_SOURCES = phoenixcontact_modbus.c
phoenixcontact_modbus_LDADD = $(LDADD_DRIVERS) $(LIBMODBUS_LIBS)
generic_modbus_SOURCES = generic_modbus.c
generic_modbus_LDADD = $(LDADD_DRIVERS) $(LIBMODBUS_LIBS)
adelsystem_cbi_SOURCES = adelsystem_cbi.c
adelsystem_cbi_LDADD = $(LDADD_DRIVERS) $(LIBMODBUS_LIBS)

# APC Modbus driver (with support of modbus over different media)
# Note that a version of libmodbus built with USB support is also needed
# for USB connections. Legacy versions work for Serial and TCP links.
apc_modbus_SOURCES = apc_modbus.c
apc_modbus_LDADD = $(LDADD_DRIVERS) $(LIBMODBUS_LIBS)
if WITH_MODBUS_USB
  apc_modbus_SOURCES += $(LIBUSB_IMPL) hidparser.c usb-common.c
  apc_modbus_LDADD += $(LIBUSB_LIBS)
endif WITH_MODBUS_USB

# Huawei UPS2000 driver
# (this is both a Modbus and a serial driver)
huawei_ups2000_SOURCES = huawei-ups2000.c
huawei_ups2000_LDADD = $(LDADD_DRIVERS_SERIAL) $(LIBMODBUS_LIBS)

# Socomec JBUS driver
# (this is a Modbus driver)
socomec_jbus_SOURCES = socomec_jbus.c
socomec_jbus_LDADD = $(LDADD_DRIVERS_SERIAL) $(LIBMODBUS_LIBS)

# Linux I2C drivers
asem_LDADD = $(LDADD_DRIVERS) $(LIBI2C_LIBS)
asem_SOURCES = asem.c
pijuice_LDADD = $(LDADD_DRIVERS) $(LIBI2C_LIBS)
pijuice_SOURCES = pijuice.c
hwmon_ina219_LDADD = $(LDADD_DRIVERS)
hwmon_ina219_SOURCES = hwmon_ina219.c

# GPIO drivers
generic_gpio_libgpiod_SOURCES = generic_gpio_common.c generic_gpio_libgpiod.c
generic_gpio_libgpiod_LDADD = $(LDADD_DRIVERS) $(LIBGPIO_LIBS)

# nutdrv_qx USB/Serial
nutdrv_qx_SOURCES = nutdrv_qx.c
nutdrv_qx_LDADD = $(LDADD_DRIVERS) -lm
nutdrv_qx_CFLAGS = $(AM_CFLAGS)
if WITH_SERIAL
nutdrv_qx_CFLAGS += -DQX_SERIAL
nutdrv_qx_LDADD += libdummy_serial.la $(SERLIBS)
endif WITH_SERIAL
if WITH_USB
nutdrv_qx_CFLAGS += -DQX_USB
nutdrv_qx_SOURCES += $(LIBUSB_IMPL) usb-common.c
nutdrv_qx_LDADD += $(LIBUSB_LIBS)
endif WITH_USB
NUTDRV_QX_SUBDRIVERS = nutdrv_qx_bestups.c nutdrv_qx_blazer-common.c	\
 nutdrv_qx_innovart31.c	\
 nutdrv_qx_innovart33.c	\
 nutdrv_qx_masterguard.c	\
 nutdrv_qx_mecer.c nutdrv_qx_megatec.c nutdrv_qx_megatec-old.c	\
 nutdrv_qx_mustek.c nutdrv_qx_q1.c nutdrv_qx_q2.c nutdrv_qx_q6.c nutdrv_qx_voltronic.c	\
 nutdrv_qx_voltronic-qs.c nutdrv_qx_voltronic-qs-hex.c nutdrv_qx_zinto.c	\
 nutdrv_qx_hunnox.c nutdrv_qx_ablerex.c nutdrv_qx_gtec.c
nutdrv_qx_SOURCES += $(NUTDRV_QX_SUBDRIVERS)

# ----------------------------------------------------------------------
# List of header files. The purpose of this list is not dependency
# tracking (which is automatic), but to ensure these files are
# distributed by "make dist".

dist_noinst_HEADERS = \
 apc_modbus.h apc-mib.h apc-iem-mib.h apc-hid.h arduino-hid.h baytech-mib.h baytech-rpc3nc-mib.h bcmxcp.h bcmxcp_ser.h	\
 bcmxcp_io.h belkin.h belkin-hid.h bestpower-mib.h blazer.h cps-hid.h dstate.h	\
 dummy-ups.h explore-hid.h failover.h gamatronic.h genericups.h	\
 generic_gpio_common.h generic_gpio_libgpiod.h	\
 hidparser.h hidtypes.h ietf-mib.h libhid.h libshut.h nut_libusb.h liebert-hid.h	\
 main.h mge-hid.h mge-mib.h mge-utalk.h		\
 mge-xml.h microdowell.h microsol-apc.h microsol-common.h netvision-mib.h netxml-ups.h nut-ipmi.h oneac.h		\
 powercom.h powerpanel.h powerp-bin.h powerp-txt.h powervar_cx.h raritan-pdu-mib.h	\
 safenet.h serial.h sms_ser.h snmp-ups.h solis.h tripplite.h tripplite-hid.h 			\
 upshandler.h usb-common.h usbhid-ups.h powercom-hid.h compaq-mib.h idowell-hid.h \
 apcsmart.h apcsmart_tabs.h apcsmart-old.h apcupsd-ups.h cyberpower-mib.h riello.h openups-hid.h \
 delta_ups-mib.h nutdrv_qx.h nutdrv_qx_bestups.h nutdrv_qx_blazer-common.h	\
 nutdrv_qx_gtec.h nutdrv_qx_innovart31.h nutdrv_qx_innovart33.h nutdrv_qx_masterguard.h nutdrv_qx_mecer.h nutdrv_qx_ablerex.h	\
 nutdrv_qx_megatec.h nutdrv_qx_megatec-old.h nutdrv_qx_mustek.h nutdrv_qx_q1.h nutdrv_qx_q2.h nutdrv_qx_q6.h nutdrv_qx_hunnox.h	\
 nutdrv_qx_voltronic.h nutdrv_qx_voltronic-qs.h nutdrv_qx_voltronic-qs-hex.h nutdrv_qx_zinto.h \
 upsdrvquery.h \
 xppc-mib.h huawei-mib.h eaton-ats16-nmc-mib.h eaton-ats16-nm2-mib.h apc-ats-mib.h raritan-px2-mib.h eaton-ats30-mib.h \
 apc-pdu-mib.h apc-epdu-mib.h ecoflow-hid.h ever-hid.h eaton-pdu-genesis2-mib.h eaton-pdu-marlin-mib.h eaton-pdu-marlin-helpers.h \
 eaton-pdu-pulizzi-mib.h eaton-pdu-revelation-mib.h emerson-avocent-pdu-mib.h eaton-ups-pwnm2-mib.h eaton-ups-pxg-mib.h legrand-hid.h \
 hpe-pdu-mib.h hpe-pdu3-cis-mib.h powervar-hid.h delta_ups-hid.h generic_modbus.h salicru-hid.h adelsystem_cbi.h eaton-pdu-nlogic-mib.h ydn23.h

# Define a dummy library so that Automake builds rules for the
# corresponding object files.  This library is not actually built,
# as a final product. It was necessary for Automake-technical reasons,
# because per-object CFLAGS can only be specified for libraries, not
# for object files. This library is used during the build process,
# and is not meant to be installed.
EXTRA_LTLIBRARIES = libdummy.la libdummy_serial.la libdummy_upsdrvquery.la

libdummy_la_SOURCES = main.c dstate.c
libdummy_la_LDFLAGS = -no-undefined -static
libdummy_serial_la_SOURCES = serial.c
libdummy_serial_la_LDFLAGS = -no-undefined -static
libdummy_upsdrvquery_la_SOURCES = upsdrvquery.c
libdummy_upsdrvquery_la_LDFLAGS = -no-undefined -static

# The libdummy_mockdrv.la is used for unit-tests to mock a driver
# with near-production codebase but without its standard main().
# Otherwise, also not meant to be installed.
EXTRA_LTLIBRARIES += libdummy_mockdrv.la
libdummy_mockdrv_la_SOURCES = main.c dstate.c
libdummy_mockdrv_la_CFLAGS = $(AM_CFLAGS) -DDRIVERS_MAIN_WITHOUT_MAIN=1
libdummy_mockdrv_la_LDFLAGS = \
	-static \
	$(top_builddir)/common/libcommon.la \
	$(top_builddir)/common/libcommonversion.la \
	$(top_builddir)/common/libparseconf.la

# Also define a library with serial-port UPS routines needed for nut-scanner
noinst_LTLIBRARIES = libserial-nutscan.la

libserial_nutscan_la_SOURCES = serial.c bcmxcp_ser.c
libserial_nutscan_la_LDFLAGS =
libserial_nutscan_la_LIBADD = $(SERLIBS)
libserial_nutscan_la_CFLAGS = $(AM_CFLAGS) -I$(top_srcdir)/clients -I$(top_srcdir)/include -I$(top_srcdir)/drivers

dummy:

CLEANFILES = $(EXTRA_LTLIBRARIES) $(EXTRA_PROGRAMS)
MAINTAINERCLEANFILES = Makefile.in .dirstamp

# NOTE: Do not clean ".deps" in SUBDIRS of the main project,
# the root Makefile.am takes care of that!
#clean-local:
#	$(AM_V_at)rm -rf $(builddir)/.deps

# Helper for only the enabled libs to get built:
all-libs-local: $(lib_LTLIBRARIES) $(noinst_LTLIBRARIES) $(EXTRA_LTLIBRARIES)
