# You need to set the environment variable: PKG_CONFIG_PATH
packagesExist(RabbitCommon) {
    pkg_config = $$pkgConfigExecutable()
    INCLUDEPATH *= $$system($$pkg_config --variable=includedir RabbitCommon)
    LIBS *= $$system($$pkg_config --libs RabbitCommon)
} else {
    isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$(RabbitCommon_DIR)
    isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$PWD/../../RabbitCommon
    !isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/Src.pro"){
        include("$${RabbitCommon_DIR}/Src/Src.pro")
    } else{
        message("Don't find RabbitCommon, in environment variable RabbitCommon_DIR:$$RabbitCommon_DIR")
        message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
        message("   git clone https://github.com/KangLin/RabbitCommon.git")
        error  ("2. Then set environment variable RabbitCommon_DIR to the download directory")
    }
}
