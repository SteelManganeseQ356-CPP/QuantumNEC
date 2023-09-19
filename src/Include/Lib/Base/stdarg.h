#ifndef _STDARG_H
#ifndef _ANSI_STDARG_H_
#ifndef __need___va_list
#define _STDARG_H
#define _ANSI_STDARG_H_
#endif /* not __need___va_list */
#undef __need___va_list

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

#ifdef _STDARG_H

#if defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L
#define va_start( v, ... ) __builtin_va_start( v, 0 )
#else
#define va_start( v, l ) __builtin_va_start( v, l )
#endif
#define va_end( v ) __builtin_va_end( v )
#define va_arg( v, l ) __builtin_va_arg( v, l )
#if !defined( __STRICT_ANSI__ ) || __STDC_VERSION__ + 0 >= 199900L \
    || __cplusplus + 0 >= 201103L
#define va_copy( d, s ) __builtin_va_copy( d, s )
#endif
#define __va_copy( d, s ) __builtin_va_copy( d, s )

#ifdef _BSD_VA_LIST
#undef _BSD_VA_LIST
#endif

#if defined( __svr4__ ) || ( defined( _SCO_DS ) && !defined( __VA_LIST ) )

#ifndef _VA_LIST_
#define _VA_LIST_
#ifdef __i860__
#ifndef _VA_LIST
#define _VA_LIST va_list
#endif
#endif /* __i860__ */
typedef __gnuc_va_list va_list;
#ifdef _SCO_DS
#define __VA_LIST
#endif
#endif /* _VA_LIST_ */
#else  /* not __svr4__ || _SCO_DS */

#if !defined( _VA_LIST_ ) || defined( __BSD_NET2__ ) || defined( ____386BSD____ ) || defined( __bsdi__ ) || defined( __sequent__ ) || defined( __FreeBSD__ ) || defined( WINNT )
/* The macro _VA_LIST_DEFINED is used in Windows NT 3.5  */
#ifndef _VA_LIST_DEFINED
/* The macro _VA_LIST is used in SCO Unix 3.2.  */
#ifndef _VA_LIST
/* The macro _VA_LIST_T_H is used in the Bull dpx2  */
#ifndef _VA_LIST_T_H
/* The macro __va_list__ is used by BeOS.  */
#ifndef __va_list__
typedef __gnuc_va_list va_list;
#endif /* not __va_list__ */
#endif /* not _VA_LIST_T_H */
#endif /* not _VA_LIST */
#endif /* not _VA_LIST_DEFINED */
#if !( defined( __BSD_NET2__ ) || defined( ____386BSD____ ) || defined( __bsdi__ ) || defined( __sequent__ ) || defined( __FreeBSD__ ) )
#define _VA_LIST_
#endif
#ifndef _VA_LIST
#define _VA_LIST
#endif
#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
#endif
#ifndef _VA_LIST_T_H
#define _VA_LIST_T_H
#endif
#ifndef __va_list__
#define __va_list__
#endif

#endif /* not _VA_LIST_, except on certain systems */

#endif /* not __svr4__ */

#if defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L
#define __STDC_VERSION_STDARG_H__ 202311L
#endif

#endif /* _STDARG_H */

#endif /* not _ANSI_STDARG_H_ */
#endif /* not _STDARG_H */
