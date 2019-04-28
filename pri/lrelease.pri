# Automatically generate .qm files out of .ts files in TRANSLATIONS and
# EXTRA_TRANSLATIONS.
#
# If embed_translations is enabled, the generated .qm files are made available
# in the resource system under :/i18n/.
#
# Otherwise, the .qm files are available in the build directory in LRELEASE_DIR.
# They can also be automatically installed by setting QM_FILES_INSTALL_PATH.

# variable, default, [suffix for variable for system() use], [prepare primary variable for system() use]
defineTest(qtPrepareTool) {
    cmd = $$eval(QT_TOOL.$${2}.binary)
    isEmpty(cmd) {
        cmd = $$[QT_HOST_BINS]/$$2
        exists($${cmd}.pl) {
            $${1}_EXE = $${cmd}.pl
            cmd = perl -w $$system_path($${cmd}.pl)
        } else: contains(QMAKE_HOST.os, Windows) {
            $${1}_EXE = $${cmd}.exe
            cmd = $$system_path($${cmd}.exe)
        } else:contains(QMAKE_HOST.os, Darwin) {
            BUNDLENAME = $${cmd}.app/Contents/MacOS/$$2
            exists($$BUNDLENAME) {
                cmd = $$BUNDLENAME
            }
            $${1}_EXE = $$cmd
        } else {
            $${1}_EXE = $$cmd
        }
    } else {
        $${1}_EXE = $$last(cmd)
    }
    export($${1}_EXE)
    QT_TOOL_ENV += $$eval(QT_TOOL.$${2}.envvars)
    QT_TOOL_NAME = $$2
    !isEmpty(3)|!isEmpty(4) {
        $$1$$3 =
        for (arg, cmd): \
            $$1$$3 += $$system_quote($$arg)
        qtAddTargetEnv($$1$$3, QT_TOOL.$${2}.depends, system)
    }
    isEmpty(4) {
        $$1 =
        for (arg, cmd): \
            $$1 += $$shell_quote($$arg)
        qtAddTargetEnv($$1, QT_TOOL.$${2}.depends, )
    }
}

# http://www.w3.org/TR/xml/#syntax
defineReplace(xml_escape) {
    1 ~= s,&,&amp;,
    1 ~= s,\',&apos;,
    1 ~= s,\",&quot;,
    1 ~= s,<,&lt;,
    1 ~= s,>,&gt;,
    return($$1)
}

qtPrepareTool(QMAKE_LRELEASE, lrelease)

isEmpty(TRANSLATIONS_NAME) : TRANSLATIONS_NAME = $${TARGET}
isEmpty(LRELEASE_DIR): LRELEASE_DIR = .qm
isEmpty(QM_FILES_RESOURCE_PREFIX): QM_FILES_RESOURCE_PREFIX = i18n
isEmpty(RCC_DIR):RCC_DIR = .

lrelease.name = lrelease
lrelease.input = TRANSLATIONS EXTRA_TRANSLATIONS
lrelease.output = $$LRELEASE_DIR/${QMAKE_FILE_IN_BASE}.qm
lrelease.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} $$QMAKE_LRELEASE_FLAGS -qm ${QMAKE_FILE_OUT}
silent: lrelease.commands = @echo lrelease ${QMAKE_FILE_IN} && $$lrelease.commands
lrelease.CONFIG = no_link target_predeps no_clean
QMAKE_EXTRA_COMPILERS += lrelease

all_translations = $${TRANSLATIONS_TS_FILES}
QM_FILES=
for (translation, all_translations) {
    # mirrors $$LRELEASE_DIR/${QMAKE_FILE_IN_BASE}.qm above
    translation = $$basename(translation)
    QM_FILES += $$OUT_PWD/$$LRELEASE_DIR/$$replace(translation, \\..*$, .qm)
}

embed_translations {    
    RESOURCE_QRC_FILE = $$RCC_DIR/translations_$${TRANSLATIONS_NAME}.qrc

    RESOURCE_QRC_FILE_CONTENT = \
                "<!DOCTYPE RCC><RCC version=\"1.0\">" \
                "<qresource prefix=\"$$xml_escape($$QM_FILES_RESOURCE_PREFIX)\">"
    abs_base = $$absolute_path($$eval($$OUT_PWD/$$LRELEASE_DIR), $$_PRO_FILE_PWD_)
    for(file, QM_FILES) {
            abs_path = $$absolute_path($$file, $$_PRO_FILE_PWD_)
            for (file, abs_path) {
                exists($$file/*): next()  # exclude directories
                alias = $$basename(file)
                RESOURCE_QRC_FILE_CONTENT += \
                    "<file alias=\"$$xml_escape($$alias)\">$$xml_escape($$file)</file>"
            }
        }
    RESOURCE_QRC_FILE_CONTENT += \
            "</qresource>" \
            "</RCC>"
    !write_file($$OUT_PWD/$$RESOURCE_QRC_FILE, RESOURCE_QRC_FILE_CONTENT): \
            error()
    RESOURCES += $$OUT_PWD/$$RESOURCE_QRC_FILE
} else {
    !isEmpty(QM_FILES_INSTALL_PATH) {
        qm_files.files += $$QM_FILES
        qm_files.path = $$QM_FILES_INSTALL_PATH
        qm_files.CONFIG = no_check_exist 
        INSTALLS += qm_files
    }
}
