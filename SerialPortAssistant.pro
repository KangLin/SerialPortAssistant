TEMPLATE = subdirs
DESTDIR = $$OUT_PWD/bin

RabbitCommon.file = 3th_libs/RabbitCommon.pro
Src.file = Src.pro
Src.depends = RabbitCommon
CONFIG *= ordered
SUBDIRS = RabbitCommon Src

