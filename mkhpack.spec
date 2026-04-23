Name:           mkhpack
Version:        1.0.0
Release:        1%{?dist}
Summary:        HPACK header compression for HTTP/2
License:        ISC
URL:            https://github.com/phluid61/mk-hpack
Source0:        %{name}-%{version}-src.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  ruby

%description
A C implementation of HPACK: Header Compression for HTTP/2 as
specified in RFC 7541. Provides integer encoding/decoding,
Huffman string encoding/decoding, and HPACK string primitives.

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
Header files, static library, and pkg-config file for developing
applications that use %{name}.

%prep
%setup -q

%build
make lib %{?_smp_mflags}

%install
make install DESTDIR=%{buildroot} PREFIX=%{_prefix}

%check
make check

%files
%license LICENSE
%doc README.md
%{_libdir}/libmkhpack.so

%files devel
%{_includedir}/mkhpack.h
%{_libdir}/libmkhpack.a
%{_libdir}/pkgconfig/mkhpack.pc

%changelog
* Wed Apr 23 2026 Matthew Kerwin <matthew@kerwin.net.au> - 1.0.0-1
- Initial package
