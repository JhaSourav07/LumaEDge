Name:           lumaedge
Version:        0.1.0
Release:        1%{?dist}
Summary:        Virtual Ring Light for Linux

License:        MIT
URL:            https://github.com/JhaSourav07/ringlight
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  cmake
BuildRequires:  ninja-build
BuildRequires:  qt6-qtbase-devel
BuildRequires:  qt6-qtdeclarative-devel
BuildRequires:  qt6-qtshadertools-devel
BuildRequires:  wayland-devel
BuildRequires:  gstreamer1-devel
BuildRequires:  gstreamer1-plugins-base-devel
BuildRequires:  desktop-file-utils
BuildRequires:  libappstream-glib

Requires:       qt6-qtbase
Requires:       qt6-qtdeclarative
Requires:       qt6-qtshadertools
Requires:       gstreamer1

%description
LumaEdge is a native Linux virtual ring light inspired by Apple's macOS
Edge Light. It illuminates the edges of your display with soft, diffused
lighting to enhance video conferencing, streaming, and online calls in
low-light environments.

%prep
%autosetup

%build
%cmake -G Ninja -DCMAKE_BUILD_TYPE=Release
%cmake_build

%install
%cmake_install

%check
%ctest

%files
%license LICENSE
%doc README.md CHANGELOG.md
%{_bindir}/lumaedge
%{_datadir}/applications/lumaedge.desktop
%{_datadir}/icons/hicolor/scalable/apps/lumaedge.svg
%{_datadir}/metainfo/com.lumaedge.LumaEdge.metainfo.xml

%changelog
* Wed Nov 04 2026 Sourav Jha <souravkjha2007@gmail.com> - 0.1.0-1
- Initial release of LumaEdge
