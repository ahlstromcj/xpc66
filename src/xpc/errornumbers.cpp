/*
 *  This file is part of xpc66.
 *
 *  xpc66 is free software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation; either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  xpc66 is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with xpc66; if not, write to the Free Software Foundation, Inc., 59 Temple
 *  Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          errornumbers.cpp
 *
 *  This module provides strings for errno values.
 *
 * \library       xpc66
 * \author        Chris Ahlstrom
 * \date          2025-10-02
 * \updates       2025-10-05
 * \license       GNU GPLv2 or above
 *
 *  Note that these strings are NOT the messages provided by perror().
 */

#include <cstring>                      /* std::strerror(3); not <cstdio>   */
#include <map>                          /* std::map()                       */

#include "xpc/errornumbers.hpp"         /* xpc::errornumbers                */

namespace xpc
{

/**
 *  A simple type.
 */

using errno_list = std::map<int, std::string>;

/**
 *  /usr/include/asm-generic/errno-base.h range is 1 to 34.
 *  /usr/include/asm-generic/errno.h range is 35 to 133.
 */

static errno_list s_error_numbers
{
    {    1,  "EPERM"              },
    {    2,  "ENOENT"             },
    {    3,  "ESRCH"              },
    {    4,  "EINTR"              },
    {    5,  "EIO"                },
    {    6,  "ENXIO"              },
    {    7,  "E2BIG"              },
    {    8,  "ENOEXEC"            },
    {    9,  "EBADF"              },
    {   10,  "ECHILD"             },
    {   11,  "EAGAIN"             },
    {   12,  "ENOMEM"             },
    {   13,  "EACCES"             },
    {   14,  "EFAULT"             },
    {   15,  "ENOTBLK"            },
    {   16,  "EBUSY"              },
    {   17,  "EEXIST"             },
    {   18,  "EXDEV"              },
    {   19,  "ENODEV"             },
    {   20,  "ENOTDIR"            },
    {   21,  "EISDIR"             },
    {   22,  "EINVAL"             },
    {   23,  "ENFILE"             },
    {   24,  "EMFILE"             },
    {   25,  "ENOTTY"             },
    {   26,  "ETXTBSY"            },
    {   27,  "EFBIG"              },
    {   28,  "ENOSPC"             },
    {   29,  "ESPIPE"             },
    {   30,  "EROFS"              },
    {   31,  "EMLINK"             },
    {   32,  "EPIPE"              },
    {   33,  "EDOM"               },
    {   34,  "ERANGE"             },
    {   35,  "EDEADLK"            },
    {   36,  "ENAMETOOLONG"       },
    {   37,  "ENOLCK"             },
    {   38,  "ENOSYS"             },
    {   39,  "ENOTEMPTY"          },
    {   40,  "ELOOP"              },
    {   41,  "EWOULDBLOCK"        },    // the actual integer is EAGAIN = 11
    {   42,  "ENOMSG"             },
    {   43,  "EIDRM"              },
    {   44,  "ECHRNG"             },
    {   45,  "EL2NSYNC"           },
    {   46,  "EL3HLT"             },
    {   47,  "EL3RST"             },
    {   48,  "ELNRNG"             },
    {   49,  "EUNATCH"            },
    {   50,  "ENOCSI"             },
    {   51,  "EL2HLT"             },
    {   52,  "EBADE"              },
    {   53,  "EBADR"              },
    {   54,  "EXFULL"             },
    {   55,  "ENOANO"             },
    {   56,  "EBADRQC"            },
    {   57,  "EBADSLT"            },
    {   58,  "EDEADLOCK"          },    // the actual integer is EDEADLK = 35
    {   59,  "EBFONT"             },
    {   60,  "ENOSTR"             },
    {   61,  "ENODATA"            },
    {   62,  "ETIME"              },
    {   63,  "ENOSR"              },
    {   64,  "ENONET"             },
    {   65,  "ENOPKG"             },
    {   66,  "EREMOTE"            },
    {   67,  "ENOLINK"            },
    {   68,  "EADV"               },
    {   69,  "ESRMNT"             },
    {   70,  "ECOMM"              },
    {   71,  "EPROTO"             },
    {   72,  "EMULTIHOP"          },
    {   73,  "EDOTDOT"            },
    {   74,  "EBADMSG"            },
    {   75,  "EOVERFLOW"          },
    {   76,  "ENOTUNIQ"           },
    {   77,  "EBADFD"             },
    {   78,  "EREMCHG"            },
    {   79,  "ELIBACC"            },
    {   80,  "ELIBBAD"            },
    {   81,  "ELIBSCN"            },
    {   82,  "ELIBMAX"            },
    {   83,  "ELIBEXEC"           },
    {   84,  "EILSEQ"             },
    {   85,  "ERESTART"           },
    {   86,  "ESTRPIPE"           },
    {   87,  "EUSERS"             },
    {   88,  "ENOTSOCK"           },
    {   89,  "EDESTADDRREQ"       },
    {   90,  "EMSGSIZE"           },
    {   91,  "EPROTOTYPE"         },
    {   92,  "ENOPROTOOPT"        },
    {   93,  "EPROTONOSUPPORT"    },
    {   94,  "ESOCKTNOSUPPORT"    },
    {   95,  "EOPNOTSUPP"         },
    {   96,  "EPFNOSUPPORT"       },
    {   97,  "EAFNOSUPPORT"       },
    {   98,  "EADDRINUSE"         },
    {   99,  "EADDRNOTAVAIL"      },
    {  100,  "ENETDOWN"           },
    {  101,  "ENETUNREACH"        },
    {  102,  "ENETRESET"          },
    {  103,  "ECONNABORTED"       },
    {  104,  "ECONNRESET"         },
    {  105,  "ENOBUFS"            },
    {  106,  "EISCONN"            },
    {  107,  "ENOTCONN"           },
    {  108,  "ESHUTDOWN"          },
    {  109,  "ETOOMANYREFS"       },
    {  110,  "ETIMEDOUT"          },
    {  111,  "ECONNREFUSED"       },
    {  112,  "EHOSTDOWN"          },
    {  113,  "EHOSTUNREACH"       },
    {  114,  "EALREADY"           },
    {  115,  "EINPROGRESS"        },
    {  116,  "ESTALE"             },
    {  117,  "EUCLEAN"            },
    {  118,  "ENOTNAM"            },
    {  119,  "ENAVAIL"            },
    {  120,  "EISNAM"             },
    {  121,  "EREMOTEIO"          },
    {  122,  "EDQUOT"             },
    {  123,  "ENOMEDIUM"          },
    {  124,  "EMEDIUMTYPE"        },
    {  125,  "ECANCELED"          },
    {  126,  "ENOKEY"             },
    {  127,  "EKEYEXPIRED"        },
    {  128,  "EKEYREVOKED"        },
    {  129,  "EKEYREJECTED"       },
    {  130,  "EOWNERDEAD"         },
    {  131,  "ENOTRECOVERABLE"    },
    {  132,  "ERFKILL"            },
    {  133,  "EHWPOISON"          }
};

/**
 *  Looks up the short name of the error number. If out-of-range,
 *  an empty string is returned.
 */

const std::string &
errno_name (int errnum)
{
    static std::string s_dummy { };
    if (errnum > 0 && errnum <= 133)
        return s_error_numbers[errnum];
    else
        return s_dummy;
}

/**
 *  A brute-force lookup of the errno associated with the given string.
 *  If not found, then 0 is returned.
 */

int
errno_number (const std::string & errname)
{
    int result { 0 };
    for (const auto & n : s_error_numbers)
    {
        if (n.second == errname)
        {
            result = n.first;
            break;
        }
    }
    return result;
}

std::string
errno_line (int e)
{
    std::string result { };
    std::string digits { std::to_string(e) };
    std::string spaces { };
    if (digits.length() == 1)
        spaces = "  ";
    else if (digits.length() == 2)
        spaces = " ";

    result += spaces;                   /* yes, we know it's krufty */
    result += digits;
    result += ": ";
    result += errno_name(e);
    result += " (";
    result += std::strerror(e);
    result += ")";
    return result;
}

/**
 *  Creates a complete error chart as a string.
 */

std::string
errno_chart ()
{
    std::string result { };
    for (const auto & n : s_error_numbers)
    {
#if USE_THIS_CODE
        std::string digits { std::to_string(n.first) };
        std::string spaces { };
        if (digits.length() == 1)
            spaces = "  ";
        else if (digits.length() == 2)
            spaces = " ";

        result += spaces;                   /* yes, we know it's krufty */
        result += std::to_string(n.first);
        result += ": ";
        result += n.second;
        result += "(";
        result += std::strerror(n.first);
#endif
        result += errno_line(n.first);
        result += "\n";
    }
    return result;
}

}               // namespace xpc

/*
 * errornumbers.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

