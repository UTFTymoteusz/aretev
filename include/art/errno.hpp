#pragma once

namespace art {
    enum error_t {
        ENONE = 0,       // Success
        E2BIG,           // Argument list too long
        EACCES,          // Permission denied
        EADDRINUSE,      // Address already in use
        EADDRNOTAVAIL,   // Cannot assign requested address
        EAGAIN,          // Try again
        EBADF,           // Bad file number
        EBADMSG,         // Bad message
        EBOTHER,         // Cannot be bothered to implement
        EBUSY,           // Device or resource busy
        ECANCELED,       // Operation canceled
        ECHILD,          // No child processes
        ECONNABORTED,    // Software caused connection abort
        ECONNREFUSED,    // Connection refused
        ECONNRESET,      // Connection reset by peer
        EDEADLK,         // Resource deadlock would occur
        EDESTADDRREQ,    // Destination address required
        EDOM,            // Mathematics argument out of domain of function
        EDQUOT,          // Reserved
        EEXIST,          // File exists
        EFAULT,          // Bad address
        EFBIG,           // File too large
        EHOSTUNREACH,    // No route to host
        EIDRM,           // Identifier removed
        EILSEQ,          // Illegal byte sequence
        EINPROGRESS,     // Operation in progress
        EINTR,           // Interrupted system call
        EINVAL,          // Invalid argument
        EIO,             // I/O error
        EISCONN,         // Transport endpoint is already connected
        EISDIR,          // Is a directory
        ELOOP,           // Too many levels of symbolic links
        EMFILE,          // File descriptor value too large
        EMLINK,          // Too many links
        EMSGSIZE,        // Message too large
        EMULTIHOP,       // Reserved
        ENAMETOOLONG,    // Filename too long
        ENETDOWN,        // Network is down
        ENETRESET,       // Network dropped connection because of reset
        ENETUNREACH,     // Network is unreachable
        ENFILE,          // Too many files open in system
        ENOBUFS,         // No buffer space available
        ENODATA,         // No message is available on the STREAM head read queue
        ENODEV,          // No such device
        ENOENT,          // No such file or directory
        ENOEXEC,         // Exec format error
        ENOLINK,         // Reserved
        ENOMEM,          // Out of memory
        ENOMSG,          // No message of the desired type
        ENOPROTOOPT,     // Protocol not available
        ENOSPC,          // No space left on device
        ENOSR,           // No STREAM resources
        ENOSTR,          // Not a STREAM
        ENOSYS,          // Not implemented
        ENOTBLK,         // Block device required
        ENOTCONN,        // Transport endpoint is not connected
        ENOTDIR,         // Not a directory
        ENOTEMPTY,       // Directory not empty
        ENOTRECOVERABLE, // State not recoverable
        ENOTSOCK,        // Not a socket
        ENOTSUP,         // Not supported
        ENOTTY,          // Iappropriate I/O control operation
        ENXIO,           // No such device of address
        EOPNOTSUPP,      // Operation not supported on socket
        EOVERFLOW,       // Value too large to be stored in data type
        EOWNERDEAD,      // Previous owner died
        EPERM,           // Operation not permitted
        EPIPE,           // Broken pipe
        EPROTO,          // Protocol error
        EPROTONOSUPPORT, // Protocol not supported
        EPROTOTYPE,      // Protocol wrong type for socket
        ERANGE,          // Result too large
        EROFS,           // Read-only filesystem
        ESPIPE,          // Invalid seek
        ESHUTDOWN,       // Cannot send after transport endpoint shutdown
        ESOCKTNOSUPPORT, // Socket type not supported
        ESRCH,           // No such process
        ETIMEDOUT,       // Connection timed out
        EAFNOSUPPORT,    // Address family not supported
    };

    const char* strerror(int num);
}