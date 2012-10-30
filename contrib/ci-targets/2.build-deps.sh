#!/usr/bin/env bash
#
# 2.build-deps.sh :
#
# Build project, including dependencies for given platform (1st argument)
# (for a list of valid platforms, see 1.env-setup.sh)
#

SOURCE_DESTDIR=/home/terracoin/dependencies
TARGET_PLATFORMS=("mingw32")

exit_error() {
    echo $1;
    exit 1;
}


for CUR_PLATFORM in ${TARGET_PLATFORMS}; do
    if [ -z ${CUR_PLATFORM} ]; then
        exit_error "NO target platform given."
    fi

    # ensure platform base directory exist:
    platform_src_dir=${SOURCE_DESTDIR}/$CUR_PLATFORM
    [ -d ${platform_src_dir} ] || exit_error "INVALID platform given, or missing platformdir"

    # quite ugly case...
    case "${CUR_PLATFORM}" in
        mingw32)
            echo "Building dependencies for mingw32 platform..."

            # openssl
            # first check if we really want to rebuilt this (7 days old):
            need_rebuild=1
            if [ -f ${platform_src_dir}/openssl-1.0.1c/libcrypto.a ]; then
                echo "libcrypto.a already built, checking its oldness..."
                last_mtime=`stat -c "%Z" ${platform_src_dir}/openssl-1.0.1c/libcrypto.a`
                now_time=`date +"%s"`
                let now_time=now_time-604800
                if [ ${last_mtime} -gt ${now_time} ]; then
                    echo "libcrypto.a generated less than 7 days ago, not rebuilding..."
                    need_rebuild=0
                fi

            fi
            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building openssl..."
                cd ${platform_src_dir}/openssl-1.0.1c/ || exit_error "Failed to change to openssl-1.0.1c/ dir"
                CROSS_COMPILE="i586-mingw32msvc-" ./Configure mingw no-asm no-shared --prefix=/usr/i586-mingw32msvc || exit_error "configure failed"
                PATH=$PATH:/usr/i586-mingw32msvc/bin make depend || exit_error "depend failed"
                PATH=$PATH:/usr/i586-mingw32msvc/bin make || exit_error "make failed"
                if [ ! -f ${platform_src_dir}/openssl-1.0.1c/libcrypto.a ]; then
                    exit_error "UNABLE TO FIND generated libcrypto.a"
                fi
            fi

            # berkeley DB
            need_rebuild=1
            if [ -f ${platform_src_dir}/db-4.8.30.NC/build_unix/libdb_cxx.a ]; then
                echo "libdb_cxx.a already built, checking its oldness..."
                last_mtime=`stat -c "%Z" ${platform_src_dir}/db-4.8.30.NC/build_unix/libdb_cxx.a`
                now_time=`date +"%s"`
                let now_time=now_time-604800
                if [ ${last_mtime} -gt ${now_time} ]; then
                    echo "libdb_cxx.a generated less than 7 days ago, not rebuilding..."
                    need_rebuild=0
                fi
            fi
            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building libdb_cxx..."
                cd ${platform_src_dir}/db-4.8.30.NC/build_unix/ || exit_error "Failed to chainge to db-4.8.30.NC/build_unix/ dir"
                sh ../dist/configure --host=i586-mingw32msvc --enable-cxx --enable-mingw || exit_error "configure failed"
                make || exit_error "make failed"
                if [ ! -f ${platform_src_dir}/db-4.8.30.NC/build_unix/libdb_cxx.a ]; then
                    exit_error "UNABLE TO FIND generated libdb_cxx.a"
                fi
            fi

            # miniupnpc
            # not including for now...
            need_rebuild=0
            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building miniupnpc..."
                cd ${platform_src_dir}/miniupnpc-1.6/ || exit_error "Failed to change to miniupnpc-1.6/ dir"
                sed -i 's/CC = gcc/CC = i586-mingw32msvc-gcc/' Makefile.mingw
                sed -i 's/wingenminiupnpcstrings \$/wine \.\/wingenminiupnpcstrings \$/' Makefile.mingw
                sed -i 's/dllwrap/i586-mingw32msvc-dllwrap/' Makefile.mingw
                sed -i 's/driver-name gcc/driver-name i586-mingw32msvc-gcc/' Makefile.mingw
                AR=i586-mingw32msvc-ar make -f Makefile.mingw
            fi

            # boost
            need_rebuild=1
            if [ -f ${platform_src_dir}/boost_1_50_0/stage/lib/libboost_system-mt.a ]; then
                echo "libboost_system-mt.a already built, checking its oldness..."
                last_mtime=`stat -c "%Z" ${platform_src_dir}/boost_1_50_0/stage/lib/libboost_system-mt.a`
                now_time=`date +"%s"`
                let now_time=now_time-604800
                if [ ${last_mtime} -gt ${now_time} ]; then
                    echo "libboost_system-mt.a generated less than 7 days ago, not rebuilding..."
                    need_rebuild=0
                fi
            fi
            if [ ${need_rebuild} -eq 1 ]; then
                echo "Building boost..."
                cd ${platform_src_dir}/boost_1_50_0/ || exit_error "Failed to change to boost_1_50_0/ dir"
                ./bootstrap.sh --without-icu || exit_error "bootstrap failed"
                echo "using gcc : 4.4 : i586-mingw32msvc-g++ : <rc>i586-mingw32msvc-windres <archiver>i586-mingw32msvc-ar ;" > user-config.jam
                ./bjam toolset=gcc target-os=windows variant=release threading=multi threadapi=win32 --user-config=user-config.jam -j 2 --without-mpi --without-python -sNO_BZIP2=1 -sNO_ZLIB=1 --layout=tagged stage
            fi

            # terracoin headless daemon:
            echo "Building terracoin headless daemon..."
            cd ${WORKSPACE}/src/leveldb/ || exit_error "Failed to change to src/leveldb/"
            PATH=/usr/i586-mingw32msvc/bin/:$PATH TARGET_OS="OS_WINDOWS_CROSSCOMPILE" CXX=i586-mingw32msvc-c++ CC=i586-mingw32msvc-cc LD=i586-mingw32msvc-ld OPT="-I${platform_src_dir}/boost_1_50_0" make libmemenv.a libleveldb.a || exit_error "Failed to build leveldb"
            cd ${WORKSPACE}/src/ || exit_error "Failed to change to src/"

            sed -i "s/^MINGW_EXTRALIBS_DIR:=(.*)$/MINGW_EXTRALIBS_DIR:=${platform_src_dir}/" makefile.linux-mingw
            make -f makefile.linux-mingw || exit_error "make failed"
            /usr/i586-mingw32msvc/bin/strip terracoind.exe || exit_error "strip failed"
            [ -f ${WORKSPACE}/src/terracoind.exe ] || exit_error "UNABLE to find generated terracoind.exe"
        ;;

        *)
            exit_error "Not Yet Implemented"
        ;;
    esac

done
