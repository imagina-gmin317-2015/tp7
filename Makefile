#############################################################################
# Makefile for building: openglwindow
# Generated by qmake (3.0) (Qt 5.3.2)
# Project:  openglwindow.pro
# Template: app
# Command: /Users/lange/Qt/5.3/clang_64/bin/qmake -spec macx-g++49 -o Makefile openglwindow.pro
#############################################################################

MAKEFILE      = Makefile

first: release
install: release-install
uninstall: release-uninstall
QMAKE         = /Users/lange/Qt/5.3/clang_64/bin/qmake
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = $(COPY_FILE)
INSTALL_PROGRAM = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
SUBTARGETS    =  \
		release \
		debug


release: FORCE
	$(MAKE) -f $(MAKEFILE).Release
release-make_first: FORCE
	$(MAKE) -f $(MAKEFILE).Release 
release-all: FORCE
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: FORCE
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: FORCE
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: FORCE
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: FORCE
	$(MAKE) -f $(MAKEFILE).Release uninstall
debug: FORCE
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_first: FORCE
	$(MAKE) -f $(MAKEFILE).Debug 
debug-all: FORCE
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: FORCE
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: FORCE
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: FORCE
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: FORCE
	$(MAKE) -f $(MAKEFILE).Debug uninstall

Makefile: openglwindow.pro ../../../../Qt/5.3/clang_64/mkspecs/macx-g++49/qmake.conf ../../../../Qt/5.3/clang_64/mkspecs/features/spec_pre.prf \
		../../../../Qt/5.3/clang_64/mkspecs/qdevice.pri \
		../../../../Qt/5.3/clang_64/mkspecs/features/device_config.prf \
		../../../../Qt/5.3/clang_64/mkspecs/common/shell-unix.conf \
		../../../../Qt/5.3/clang_64/mkspecs/common/unix.conf \
		../../../../Qt/5.3/clang_64/mkspecs/common/mac.conf \
		../../../../Qt/5.3/clang_64/mkspecs/common/macx.conf \
		../../../../Qt/5.3/clang_64/mkspecs/common/gcc-base.conf \
		../../../../Qt/5.3/clang_64/mkspecs/common/gcc-base-mac.conf \
		../../../../Qt/5.3/clang_64/mkspecs/common/g++-base.conf \
		../../../../Qt/5.3/clang_64/mkspecs/common/g++-macx.conf \
		../../../../Qt/5.3/clang_64/mkspecs/qconfig.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_bluetooth.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_bluetooth_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_bootstrap_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_clucene_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_concurrent.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_concurrent_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_core.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_core_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_dbus.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_dbus_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_declarative.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_declarative_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_designer.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_designer_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_designercomponents_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_enginio.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_enginio_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_gui.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_gui_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_help.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_help_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_macextras.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_macextras_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_multimedia.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_multimedia_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_multimediawidgets.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_multimediawidgets_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_network.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_network_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_nfc.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_nfc_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_opengl.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_opengl_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_openglextensions.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_openglextensions_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_platformsupport_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_positioning.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_positioning_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_printsupport.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_printsupport_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qml.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qml_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qmldevtools_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qmltest.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qmltest_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quick.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quick_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quickparticles_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quickwidgets.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quickwidgets_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_script.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_script_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_scripttools.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_scripttools_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_sensors.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_sensors_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_serialport.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_serialport_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_sql.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_sql_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_svg.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_svg_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_testlib.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_testlib_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_uitools.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_uitools_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_webkit.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_webkit_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_webkitwidgets.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_webkitwidgets_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_websockets.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_websockets_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_widgets.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_widgets_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_xml.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_xml_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_xmlpatterns.pri \
		../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		../../../../Qt/5.3/clang_64/mkspecs/features/qt_functions.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/qt_config.prf \
		../../../../Qt/5.3/clang_64/mkspecs/macx-g++49/qmake.conf \
		../../../../Qt/5.3/clang_64/mkspecs/features/spec_post.prf \
		.qmake.stash \
		../../../../Qt/5.3/clang_64/mkspecs/features/exclusive_builds.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/default_pre.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/mac/default_pre.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/resolve_config.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/exclusive_builds_post.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/default_post.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/mac/sdk.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/mac/default_post.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/mac/objective_c.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/warn_on.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/qt.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/resources.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/moc.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/unix/opengl.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/unix/thread.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/mac/rez.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/testcase_targets.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/exceptions.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/yacc.prf \
		../../../../Qt/5.3/clang_64/mkspecs/features/lex.prf \
		openglwindow.pro \
		/Users/lange/Qt/5.3/clang_64/lib/QtGui.framework/QtGui.prl \
		/Users/lange/Qt/5.3/clang_64/lib/QtCore.framework/QtCore.prl
	$(QMAKE) -spec macx-g++49 -o Makefile openglwindow.pro
../../../../Qt/5.3/clang_64/mkspecs/features/spec_pre.prf:
../../../../Qt/5.3/clang_64/mkspecs/qdevice.pri:
../../../../Qt/5.3/clang_64/mkspecs/features/device_config.prf:
../../../../Qt/5.3/clang_64/mkspecs/common/shell-unix.conf:
../../../../Qt/5.3/clang_64/mkspecs/common/unix.conf:
../../../../Qt/5.3/clang_64/mkspecs/common/mac.conf:
../../../../Qt/5.3/clang_64/mkspecs/common/macx.conf:
../../../../Qt/5.3/clang_64/mkspecs/common/gcc-base.conf:
../../../../Qt/5.3/clang_64/mkspecs/common/gcc-base-mac.conf:
../../../../Qt/5.3/clang_64/mkspecs/common/g++-base.conf:
../../../../Qt/5.3/clang_64/mkspecs/common/g++-macx.conf:
../../../../Qt/5.3/clang_64/mkspecs/qconfig.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_bluetooth.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_bluetooth_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_bootstrap_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_clucene_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_concurrent.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_concurrent_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_core.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_core_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_dbus.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_dbus_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_declarative.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_declarative_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_designer.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_designer_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_designercomponents_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_enginio.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_enginio_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_gui.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_gui_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_help.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_help_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_macextras.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_macextras_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_multimedia.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_multimedia_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_multimediawidgets.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_multimediawidgets_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_network.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_network_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_nfc.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_nfc_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_opengl.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_opengl_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_openglextensions.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_openglextensions_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_platformsupport_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_positioning.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_positioning_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_printsupport.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_printsupport_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qml.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qml_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qmldevtools_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qmltest.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qmltest_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quick.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quick_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quickparticles_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quickwidgets.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_quickwidgets_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_script.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_script_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_scripttools.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_scripttools_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_sensors.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_sensors_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_serialport.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_serialport_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_sql.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_sql_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_svg.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_svg_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_testlib.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_testlib_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_uitools.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_uitools_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_webkit.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_webkit_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_webkitwidgets.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_webkitwidgets_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_websockets.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_websockets_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_widgets.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_widgets_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_xml.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_xml_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_xmlpatterns.pri:
../../../../Qt/5.3/clang_64/mkspecs/modules/qt_lib_xmlpatterns_private.pri:
../../../../Qt/5.3/clang_64/mkspecs/features/qt_functions.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/qt_config.prf:
../../../../Qt/5.3/clang_64/mkspecs/macx-g++49/qmake.conf:
../../../../Qt/5.3/clang_64/mkspecs/features/spec_post.prf:
.qmake.stash:
../../../../Qt/5.3/clang_64/mkspecs/features/exclusive_builds.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/default_pre.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/mac/default_pre.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/resolve_config.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/exclusive_builds_post.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/default_post.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/mac/sdk.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/mac/default_post.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/mac/objective_c.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/warn_on.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/qt.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/resources.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/moc.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/unix/opengl.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/unix/thread.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/mac/rez.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/testcase_targets.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/exceptions.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/yacc.prf:
../../../../Qt/5.3/clang_64/mkspecs/features/lex.prf:
openglwindow.pro:
/Users/lange/Qt/5.3/clang_64/lib/QtGui.framework/QtGui.prl:
/Users/lange/Qt/5.3/clang_64/lib/QtCore.framework/QtCore.prl:
qmake: FORCE
	@$(QMAKE) -spec macx-g++49 -o Makefile openglwindow.pro

qmake_all: FORCE

make_first: release-make_first debug-make_first FORCE
all: release-all debug-all FORCE
clean: release-clean debug-clean FORCE
distclean: release-distclean debug-distclean FORCE
	-$(DEL_FILE) Makefile

release-mocclean:
	$(MAKE) -f $(MAKEFILE).Release mocclean
debug-mocclean:
	$(MAKE) -f $(MAKEFILE).Debug mocclean
mocclean: release-mocclean debug-mocclean

release-mocables:
	$(MAKE) -f $(MAKEFILE).Release mocables
debug-mocables:
	$(MAKE) -f $(MAKEFILE).Debug mocables
mocables: release-mocables debug-mocables

check: first
FORCE:

$(MAKEFILE).Release: Makefile
$(MAKEFILE).Debug: Makefile
