/*
 * Copyright (C) 1999  Internet Software Consortium.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#ifndef DNS_CONFIG_CONFCOMMON_H
#define DNS_CONFIG_CONFCOMMON_H 1

/*****
 ***** Module Info
 *****/

/*
 * Various declarations of types and functions that are used by multiple
 * headers in the config file module (put here to avoid circular include
 * dependencies).
 *
 * Also some memory debugging aids that should eventually get moved to
 * isc/mem.h or removed.
 */

/*
 * MP:
 *
 *	N/A
 *
 * Reliability:
 *
 * 	No problems known.
 *
 * Resources:
 *
 *	N/A
 *
 * Security:
 *
 *	N/A
 */

/***
 *** Imports
 ***/

#include <config.h>

#include <sys/types.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

#include <isc/int.h>
#include <isc/result.h>
#include <isc/types.h>
#include <isc/mem.h>
#include <isc/net.h>
#include <isc/sockaddr.h>

#include <dns/log.h>
#include <dns/types.h>

/* Constants used in the defintions of default logging channels and
   categories */
#define DNS_C_DEFAULT_SYSLOG "default_syslog"
#define DNS_C_DEFAULT_DEBUG "default_debug"
#define DNS_C_DEFAULT_DEBUG_PATH "named.run"
#define DNS_C_NULL "null"
#define DNS_C_DEFAULT_STDERR  "default_stderr"
#define DNS_C_STDERR_PATH " <stderr> "	/* not really a path */



/* The value we use in config files if the user doesn't specify the port or 
 *   in some statements
 */
#define DNS_C_DEFAULTPORT	53	/* XXX this should be imported */


/* What an 'unlimited' value for a size_spec is stored as internally */
#define DNS_C_SIZE_SPEC_UNLIM (~((isc_uint32_t) 0x0))

/* What a 'default' value for a size_spec is stored as internally */
#define DNS_C_SIZE_SPEC_DEFAULT (DNS_C_SIZE_SPEC_UNLIM - 1)

/* What 'unlimited' is stored as internally for logging file versions */
#define DNS_C_UNLIM_VERSIONS DNS_C_SIZE_SPEC_UNLIM

/* The default ordering given to rrset-order statements when the type given 
   is illegal (so parsing can continue). */
#define DNS_DEFAULT_ORDERING dns_c_ordering_fixed



/***
 *** Types
 ***/

/* Value of a 'forward' statement */
typedef enum {
	dns_c_forw_only,
	dns_c_forw_first,
	dns_c_forw_noanswer,
	dns_c_forw_nodomain
} dns_c_forw_t;

/* value of a 'check-names' method  */
typedef enum {
	dns_c_severity_ignore,
	dns_c_severity_warn,
	dns_c_severity_fail
} dns_c_severity_t;

/* Value of a 'check-names' type. */
typedef enum {
	dns_trans_primary,
	dns_trans_secondary,
	dns_trans_response
} dns_c_trans_t ;
#define DNS_C_TRANSCOUNT 3	  /* number of items in dns_c_trans_t enum */


/* The tag values for the different types of control channels */
typedef enum {
	dns_c_inet_control,
	dns_c_unix_control
} dns_c_control_t;


/* The possible rrset-order ordering values. */
typedef enum {
	dns_c_ordering_fixed,
	dns_c_ordering_random,
	dns_c_ordering_cyclic
} dns_c_ordering_t;



#if 0
typedef enum {
	dns_one_answer, dns_many_answers
} dns_c_transferformat_t;
#endif

/* Possible zone types */
typedef enum {
	dns_c_zone_master,
	dns_c_zone_slave,
	dns_c_zone_hint,
	dns_c_zone_stub,
	dns_c_zone_forward
} dns_c_zonetype_t;


/* Possible address-match-element types */ 
typedef enum {
	dns_c_ipmatch_pattern,
	dns_c_ipmatch_indirect,
	dns_c_ipmatch_localhost,
	dns_c_ipmatch_localnets,
	dns_c_ipmatch_key,
	dns_c_ipmatch_acl,
	dns_c_ipmatch_none
} dns_c_ipmatch_type_t;


/* Tag values for the different types of log channel */
typedef enum {
	dns_c_logchan_file,
	dns_c_logchan_syslog,
	dns_c_logchan_null
} dns_c_logchantype_t;


/* Possible logging severity values */
typedef enum {
	dns_c_log_critical,
	dns_c_log_error,
	dns_c_log_warn,
	dns_c_log_notice,
	dns_c_log_info,
	dns_c_log_debug,
	dns_c_log_dynamic,
	dns_c_log_no_severity
} dns_c_logseverity_t;


/* Possible logging categories. */
typedef enum {
	dns_c_cat_default,
	dns_c_cat_config,
	dns_c_cat_parser,
	dns_c_cat_queries,  
	dns_c_cat_lameservers,
	dns_c_cat_statistics,
	dns_c_cat_panic, 
	dns_c_cat_update,
	dns_c_cat_ncache,
	dns_c_cat_xferin,
	dns_c_cat_xferout,  
	dns_c_cat_db,
	dns_c_cat_eventlib,
	dns_c_cat_packet,
	dns_c_cat_notify, 
	dns_c_cat_cname,
	dns_c_cat_security,
	dns_c_cat_os,
	dns_c_cat_insist, 
	dns_c_cat_maint,
	dns_c_cat_load,
	dns_c_cat_respchecks,
	dns_c_cat_control, 
	dns_c_cat_none
} dns_c_category_t;


/* Type of the bit sets used in various structures. Macros in confpvt.h
 * depending on this being an integer type, and some structures need more
 * than 32 bits.
 */
typedef isc_int64_t	dns_c_setbits_t;


/* XXX This should be moved to a more general (non-config specific) place */
/* An IP address. We support IPv4 and IPv6 addresses together so we wrap
   them up in this strcture*/
#if 0

typedef struct dns_c_addr {
	int	a_family;		/* AF_INET or AF_INET6 */
	union {
		struct in_addr a;	/* if a_family == AF_INET */
		struct in6_addr a6;	/* if a_family == AF_INET6 */
	} u;
} dns_c_addr_t;

#else

typedef isc_sockaddr_t dns_c_addr_t;

#endif


/*
 * Set this variable to a true value to get output by the wrapper
 * functions (if the memory debugging hack is compiled in--it isn't by
 * default
 */ 

extern isc_boolean_t debug_mem_print;
extern FILE *debug_mem_print_stream;	/* NULL means stderr */

#if 0					/* XXXJAB remove this code. */
typedef void (*dns_cfg_err_handler_t)(isc_result_t code,
				      const char *fmt, va_list args);
#endif


/***
 *** Functions
 ***/

/* The following dns_c_xxx2string() functions convert the first argument into 
 * a string value and returns that value. If the first argument is not a
 * legal value, then NULL is returned, unless PRINTABLE is true, in which 
 * case an ugly, but safe-to-pass-to-printf string is returned.
 *
 * e.g. dns_c_ordering2string(dns_c_ordering_cyclic,ISC_FALSE) returns the 
 * string "cyclic", but
 * dns_c_ordering2string((dns_c_ordering_t)0xffff,ISC_TRUE) returns the
 * value "UNKNOWN_ORDERING"
 */
const char *		dns_c_ordering2string(dns_c_ordering_t ordering,
					      isc_boolean_t printable);
const char *		dns_c_logseverity2string(dns_c_logseverity_t level,
					      isc_boolean_t printable);
const char *		dns_c_category2string(dns_c_category_t cat,
					      isc_boolean_t printable);
const char *		dns_c_facility2string(int facility,
					      isc_boolean_t printable);
const char *		dns_c_transformat2string(dns_transfer_format_t tform,
						isc_boolean_t printable);
const char *		dns_c_transport2string(dns_c_trans_t transport,
					       isc_boolean_t printable);
const char *		dns_c_nameseverity2string(dns_c_severity_t severity,
						  isc_boolean_t printable);
const char *		dns_c_forward2string(dns_c_forw_t forw,
					     isc_boolean_t printable);

/*
 * The following dns_c_string2xxx() functions will look up the string
 * argument in a table of values and will return the appropriate enum/integer 
 * through the second argument and ISC_R_SUCCESS is returned. If the string
 * doesn't match a valid value then ISC_R_FAILURE is returned.
 */
isc_result_t		dns_c_string2ordering(char *name,
					      dns_c_ordering_t *ordering);
isc_result_t		dns_c_string2logseverity(const char *string,
					      dns_c_logseverity_t *result);
isc_result_t		dns_c_string2category(const char *string,
					      dns_c_category_t *category);
isc_result_t		dns_c_string2facility(const char *string, int *res);



int			dns_c_isanyaddr(isc_sockaddr_t *inaddr);
void 			dns_c_print_ipaddr(FILE *fp, isc_sockaddr_t *addr);
isc_boolean_t		dns_c_need_quote(const char *string);

void			dns_c_printtabs(FILE *fp, int count); 
void			dns_c_printinunits(FILE *fp, isc_uint32_t val);

void			dns_c_dataclass_tostream(FILE *fp,
						 dns_rdataclass_t rclass);
void			dns_c_datatype_tostream(FILE *fp,
						dns_rdatatype_t rtype);

#if 0					/* XXXJAB remove this */
dns_cfg_err_handler_t	dns_c_seterrorhandler(dns_cfg_err_handler_t
						newhandler);
void	 		dns_c_error(isc_result_t result, const char *fmt, ...);
#endif


#if defined(DEBUG_MEM_STUFF)

/* XXX debugging stuff that should probably be moved to isc/mem.h */

#undef isc_mem_get
#undef isc_mem_put

/*
 * Some wrappers for the various mem functions to help in debugging.
 */
#define isc_mem_get(a, b) \
	dns_c_memget_wrapper(__FILE__,__LINE__, a, b)
#define isc_mem_put(a, b, c) \
	dns_c_memput_wrapper(__FILE__, __LINE__, a, b, c)
#define isc_mem_strdup(a, b) \
	dns_c_memstrdup_wrapper(__FILE__,__LINE__, a, b)
#define isc_mem_free(a, b) \
	dns_c_memfree_wrapper(__FILE__, __LINE__, a, b)
#define isc_mem_destroy(a) \
	dns_c_memdestroy_wrapper(__FILE__, __LINE__, a)


/*
 * These functions create output that's meant to be used by the
 * find_leak.pl script.
 */
void   *dns_c_memget_wrapper(const char *file, int line,
			     isc_mem_t *mem, size_t sz);
void 	dns_c_memput_wrapper(const char *file, int line,
			     isc_mem_t *mem, void *p, size_t sz);
char   *dns_c_memstrdup_wrapper(const char *file, int line,
				isc_mem_t *mem, const char *string);
void 	dns_c_memfree_wrapper(const char *file, int line,
			      isc_mem_t *mem, char *string);
void 	dns_c_memdestroy_wrapper(const char *file, int line, isc_mem_t **mem);

#endif /* defined(DEBUG_MEM_STUFF) */

#endif /* DNS_CONFIG_CONFCOMMON_H */
