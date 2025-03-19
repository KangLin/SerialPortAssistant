# See: https://rpm-software-management.github.io/rpm/manual/spec.html

Name:           serialportassistant
Version:        0.0.32
Release:        1%{?dist}
Summary:        Serial port assistant

License:        GPL-3.0
URL:            https://github.com/KangLin/SerialPortAssistant
Source0:        SerialPortAssistant.tar.gz

Vendor:         Kang Lin <kl222@126.com>

BuildRequires: make git rpm-build rpmdevtools 
BuildRequires: gcc-c++
BuildRequires: automake autoconf libtool gettext gettext-autopoint
BuildRequires: cmake desktop-file-utils appstream
BuildRequires: curl wget cmark-devel

# Qt6
BuildRequires: qt6-qttools-devel qt6-qtbase-devel qt6-qtmultimedia-devel
BuildRequires: qt6-qt5compat-devel qt6-qtmultimedia-devel qt6-qtscxml-devel
BuildRequires: qt6-qtserialport-devel qt6-qtsvg-devel

BuildRequires: openssl-devel

Requires: openssl cmark

Requires: qt6-qtbase qt6-qtmultimedia qt6-qt5compat qt6-qtmultimedia qt6-qtscxml
Requires: qt6-qtserialport qt6-qtsvg

%description
Serial port assistant
Author: Kang Lin <kl222@126.com>
Donation:
https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png
https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png
https://gitlab.com/kl222/RabbitCommon/-/raw/master/Src/Resource/image/Contribute.png

# 预备参数，通常为 %setup -q
%prep
%setup -q -n SerialPortAssistant

%generate_buildrequires

%build
echo "-- RPM_BUILD_DIR: $RPM_BUILD_DIR"
echo "-- RPM_BUILD_ROOT: $RPM_BUILD_ROOT"
echo "-- RPM_SOURCE_DIR: $RPM_SOURCE_DIR"
echo "-- RPM_SPECPARTS_DIR: $RPM_SPECPARTS_DIR"
echo "-- pwd: `pwd`"

cmake . -B $RPM_BUILD_DIR  \
    -DCMAKE_INSTALL_PREFIX=/opt/SerialPortAssistant -DCMAKE_BUILD_TYPE=Release \
    -DRabbitCommon_ROOT=${RabbitCommon_ROOT} \
    -DBUILD_FREERDP=OFF \
    -DBUILD_LibVNCServer=OFF
cmake --build $RPM_BUILD_DIR --config Release --parallel $(nproc)

%install
cmake --install $RPM_BUILD_DIR --config Release \
    --component DependLibraries \
    --prefix ${RPM_BUILD_ROOT}/opt/SerialPortAssistant
cmake --install $RPM_BUILD_DIR --config Release \
    --component Runtime \
    --prefix ${RPM_BUILD_ROOT}/opt/SerialPortAssistant
cmake --install $RPM_BUILD_DIR --config Release \
    --component Application \
    --prefix ${RPM_BUILD_ROOT}/opt/SerialPortAssistant
cmake --install $RPM_BUILD_DIR --config Release \
    --component Plugin \
    --prefix ${RPM_BUILD_ROOT}/opt/SerialPortAssistant

# 安装前需要做的任务，如：创建用户
%pre

# 安装后需要做的任务 如：自动启动的任务
%post

# 卸载前需要做的任务 如：停止任务
%preun

# 卸载后需要做的任务 如：删除用户，删除/备份业务数据
%postun

# 设置文件属性，包含编译文件需要生成的目录、文件以及分配所对应的权限
%files
/opt/SerialPortAssistant/*

# 清除上次编译生成的临时文件
%clean

# 修改历史
%changelog
* Fri Jan 01 2025 Kang Lin <kl222@126.com> - 0.0.32-1
- Initial RPM package
