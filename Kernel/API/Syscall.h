#pragma once

#include <AK/Types.h>
#include <AK/Userspace.h>

constexpr int syscall_vector = 0x82;

extern "C" {
struct pollfd;
struct timeval;
struct timespec;
struct sockaddr;
struct siginfo;
struct stat;
struct statvfs;
typedef u32 socklen_t;
}

namespace Kernel {

enum class NeedsBigProcessLock {
  Yes,
  No
};

#define ENUMERATE_SYSCALLS(S)                               \
  S(accept4, NeedsBigProcessLock::No)                     \
  S(access, NeedsBigProcessLock::Yes)                     \
  S(adjtime, NeedsBigProcessLock::No)                     \
  S(alarm, NeedsBigProcessLock::Yes)                      \
  S(allocate_tls, NeedsBigProcessLock::Yes)               \
  S(anon_create, NeedsBigProcessLock::No)                 \
  S(beep, NeedsBigProcessLock::No)                        \
  S(bind, NeedsBigProcessLock::No)                        \
  S(chdir, NeedsBigProcessLock::No)                       \
  S(chmod, NeedsBigProcessLock::No)                       \
  S(chown, NeedsBigProcessLock::No)                       \
  S(clock_gettime, NeedsBigProcessLock::No)               \
  S(clock_nanosleep, NeedsBigProcessLock::No)             \
  S(clock_getres, NeedsBigProcessLock::No)                \
  S(clock_settime, NeedsBigProcessLock::No)               \
  S(close, NeedsBigProcessLock::No)                       \
  S(connect, NeedsBigProcessLock::No)                     \
  S(create_inode_watcher, NeedsBigProcessLock::Yes)       \
  S(create_thread, NeedsBigProcessLock::Yes)              \
  S(dbgputstr, NeedsBigProcessLock::No)                   \
  S(detach_thread, NeedsBigProcessLock::Yes)              \
  S(disown, NeedsBigProcessLock::Yes)                     \
  S(dump_backtrace, NeedsBigProcessLock::No)              \
  S(dup2, NeedsBigProcessLock::No)                        \
  S(emuctl, NeedsBigProcessLock::No)                      \
  S(execve, NeedsBigProcessLock::Yes)                     \
  S(exit, NeedsBigProcessLock::Yes)                       \
  S(exit_thread, NeedsBigProcessLock::Yes)                \
  S(fchdir, NeedsBigProcessLock::No)                      \
  S(fchmod, NeedsBigProcessLock::No)                      \
  S(fchown, NeedsBigProcessLock::No)                      \
  S(fcntl, NeedsBigProcessLock::Yes)                      \
  S(fork, NeedsBigProcessLock::Yes)                       \
  S(fstat, NeedsBigProcessLock::No)                       \
  S(fstatvfs, NeedsBigProcessLock::No)                    \
  S(fsync, NeedsBigProcessLock::No)                       \
  S(ftruncate, NeedsBigProcessLock::No)                   \
  S(futex, NeedsBigProcessLock::Yes)                      \
  S(get_dir_entries, NeedsBigProcessLock::Yes)            \
  S(get_process_name, NeedsBigProcessLock::Yes)           \
  S(get_stack_bounds, NeedsBigProcessLock::No)            \
  S(get_thread_name, NeedsBigProcessLock::Yes)            \
  S(getcwd, NeedsBigProcessLock::No)                      \
  S(getegid, NeedsBigProcessLock::Yes)                    \
  S(geteuid, NeedsBigProcessLock::Yes)                    \
  S(getgid, NeedsBigProcessLock::Yes)                     \
  S(getgroups, NeedsBigProcessLock::Yes)                  \
  S(gethostname, NeedsBigProcessLock::No)                 \
  S(getkeymap, NeedsBigProcessLock::No)                   \
  S(getpeername, NeedsBigProcessLock::Yes)                \
  S(getpgid, NeedsBigProcessLock::Yes)                    \
  S(getpgrp, NeedsBigProcessLock::Yes)                    \
  S(getpid, NeedsBigProcessLock::No)                      \
  S(getppid, NeedsBigProcessLock::Yes)                    \
  S(getrandom, NeedsBigProcessLock::No)                   \
  S(getresgid, NeedsBigProcessLock::Yes)                  \
  S(getresuid, NeedsBigProcessLock::Yes)                  \
  S(getrusage, NeedsBigProcessLock::Yes)                  \
  S(getsid, NeedsBigProcessLock::Yes)                     \
  S(getsockname, NeedsBigProcessLock::Yes)                \
  S(getsockopt, NeedsBigProcessLock::No)                  \
  S(gettid, NeedsBigProcessLock::No)                      \
  S(getuid, NeedsBigProcessLock::Yes)                     \
  S(inode_watcher_add_watch, NeedsBigProcessLock::Yes)    \
  S(inode_watcher_remove_watch, NeedsBigProcessLock::Yes) \
  S(ioctl, NeedsBigProcessLock::Yes)                      \
  S(join_thread, NeedsBigProcessLock::Yes)                \
  S(kill, NeedsBigProcessLock::Yes)                       \
  S(kill_thread, NeedsBigProcessLock::Yes)                \
  S(killpg, NeedsBigProcessLock::Yes)                     \
  S(link, NeedsBigProcessLock::No)                        \
  S(listen, NeedsBigProcessLock::No)                      \
  S(lseek, NeedsBigProcessLock::No)                       \
  S(madvise, NeedsBigProcessLock::Yes)                    \
  S(map_time_page, NeedsBigProcessLock::Yes)              \
  S(mkdir, NeedsBigProcessLock::No)                       \
  S(mknod, NeedsBigProcessLock::Yes)                      \
  S(mmap, NeedsBigProcessLock::Yes)                       \
  S(mount, NeedsBigProcessLock::Yes)                      \
  S(mprotect, NeedsBigProcessLock::Yes)                   \
  S(mremap, NeedsBigProcessLock::Yes)                     \
  S(msync, NeedsBigProcessLock::Yes)                      \
  S(msyscall, NeedsBigProcessLock::Yes)                   \
  S(munmap, NeedsBigProcessLock::Yes)                     \
  S(open, NeedsBigProcessLock::Yes)                       \
  S(perf_event, NeedsBigProcessLock::Yes)                 \
  S(perf_register_string, NeedsBigProcessLock::Yes)       \
  S(pipe, NeedsBigProcessLock::No)                        \
  S(pledge, NeedsBigProcessLock::Yes)                     \
  S(poll, NeedsBigProcessLock::Yes)                       \
  S(posix_fallocate, NeedsBigProcessLock::No)             \
  S(prctl, NeedsBigProcessLock::Yes)                      \
  S(profiling_disable, NeedsBigProcessLock::Yes)          \
  S(profiling_enable, NeedsBigProcessLock::Yes)           \
  S(profiling_free_buffer, NeedsBigProcessLock::Yes)      \
  S(ptrace, NeedsBigProcessLock::Yes)                     \
  S(purge, NeedsBigProcessLock::Yes)                      \
  S(read, NeedsBigProcessLock::Yes)                       \
  S(pread, NeedsBigProcessLock::Yes)                      \
  S(readlink, NeedsBigProcessLock::No)                    \
  S(readv, NeedsBigProcessLock::Yes)                      \
  S(realpath, NeedsBigProcessLock::No)                    \
  S(recvfd, NeedsBigProcessLock::No)                      \
  S(recvmsg, NeedsBigProcessLock::Yes)                    \
  S(rename, NeedsBigProcessLock::No)                      \
  S(rmdir, NeedsBigProcessLock::No)                       \
  S(sched_getparam, NeedsBigProcessLock::No)              \
  S(sched_setparam, NeedsBigProcessLock::No)              \
  S(sendfd, NeedsBigProcessLock::No)                      \
  S(sendmsg, NeedsBigProcessLock::Yes)                    \
  S(set_coredump_metadata, NeedsBigProcessLock::No)       \
  S(set_mmap_name, NeedsBigProcessLock::Yes)              \
  S(set_process_name, NeedsBigProcessLock::Yes)           \
  S(set_thread_name, NeedsBigProcessLock::Yes)            \
  S(setegid, NeedsBigProcessLock::Yes)                    \
  S(seteuid, NeedsBigProcessLock::Yes)                    \
  S(setgid, NeedsBigProcessLock::Yes)                     \
  S(setgroups, NeedsBigProcessLock::Yes)                  \
  S(sethostname, NeedsBigProcessLock::No)                 \
  S(setkeymap, NeedsBigProcessLock::No)                   \
  S(setpgid, NeedsBigProcessLock::Yes)                    \
  S(setresgid, NeedsBigProcessLock::Yes)                  \
  S(setresuid, NeedsBigProcessLock::Yes)                  \
  S(setreuid, NeedsBigProcessLock::Yes)                   \
  S(setsid, NeedsBigProcessLock::Yes)                     \
  S(setsockopt, NeedsBigProcessLock::No)                  \
  S(setuid, NeedsBigProcessLock::Yes)                     \
  S(shutdown, NeedsBigProcessLock::No)                    \
  S(sigaction, NeedsBigProcessLock::Yes)                  \
  S(sigaltstack, NeedsBigProcessLock::Yes)                \
  S(sigpending, NeedsBigProcessLock::Yes)                 \
  S(sigprocmask, NeedsBigProcessLock::Yes)                \
  S(sigreturn, NeedsBigProcessLock::Yes)                  \
  S(sigsuspend, NeedsBigProcessLock::Yes)                 \
  S(sigtimedwait, NeedsBigProcessLock::Yes)               \
  S(socket, NeedsBigProcessLock::Yes)                     \
  S(socketpair, NeedsBigProcessLock::No)                  \
  S(stat, NeedsBigProcessLock::No)                        \
  S(statvfs, NeedsBigProcessLock::No)                     \
  S(symlink, NeedsBigProcessLock::No)                     \
  S(sync, NeedsBigProcessLock::No)                        \
  S(sysconf, NeedsBigProcessLock::No)                     \
  S(times, NeedsBigProcessLock::Yes)                      \
  S(umask, NeedsBigProcessLock::Yes)                      \
  S(umount, NeedsBigProcessLock::Yes)                     \
  S(uname, NeedsBigProcessLock::No)                       \
  S(unlink, NeedsBigProcessLock::No)                      \
  S(unveil, NeedsBigProcessLock::Yes)                     \
  S(utime, NeedsBigProcessLock::Yes)                      \
  S(utimensat, NeedsBigProcessLock::Yes)                  \
  S(waitid, NeedsBigProcessLock::Yes)                     \
  S(write, NeedsBigProcessLock::Yes)                      \
  S(writev, NeedsBigProcessLock::Yes)                     \
  S(yield, NeedsBigProcessLock::No)

namespace Syscall {
#undef __ENUMERATE_SYSCALL
#define __ENUMERATE_SYSCALL(sys_call, needs_lock) SC_##sys_call,
  ENUMERATE_SYSCALLS(__ENUMERATE_SYSCALL)
#undef __ENUMERATE_SYSCALL
    __Count
};

#ifdef __literate__
struct StringArgument {
  char const* characters;
  size_t length { 0 };
};

template<typename DataType, typename SizeType>
struct MutableBufferArgument {
  DataType* data { nullptr };
  SizeType size { 0 };
};

}
