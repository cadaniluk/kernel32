ifdef(`UTIL_M4', ,
define(`UTIL_M4')

/* My first GNU m4 file. The code is probably not perfect because of my
 * lack of experience and skill with m4 but that will improve. */


/* TODO: move to a better-suited location. OR NOT, in case it's ok here. */
/* Emits an error message and terminates the program with exit code `1'.
 *
 * Parameters:
 * `$n' - The error messages to print.
 *
 * Notes:
 * You should NOT rely on this function actually exiting m4 because I think it
 * improves the quality and stability of the code. */
define(`UTIL_FATAL', `errprint(__program__:__file__:__line__:` $*
')')

/* Emits a warning message.
 *
 * Parameters:
 * `$n' - The error messages to print. */
define(`UTIL_WARN', `errprint(__program__:__file__:__line:` $*')')

/* Emits the name of a data field.
 *
 * Parameters:
 * `$1' - The name of the structure, which contains the data field.
 * `$2' - The name of the data field. */
define(`UTIL_MEMBER', `util_`$1'.`$2'')

/* Emits the number of arguments.
 *
 * Parameters:
 * `$n' - Arbitrary arguments. */
define(`__UTIL_ARGC', `$#')

/* Checks whether the argument list's length is a multiple of `n' (`1',
 * otherwise `0').
 *
 * Parameters:
 * `$1' - `n'.
 * The other arguments are arbitrary and their values irrelevant to
 * the functioning of this macro. Their presence is crucial, though. */
define(`__UTIL_CHECK_STRUCT_ARGS',
`ifelse(eval(__UTIL_ARGC(shift($@))` % $1'), `0', `1', `0')')

/* Helper function for `UTIL_DEF'. Does the actual enumeration of data.
 *
 * Parameters:
 * `$n' - Data fields to define. */
define(`__UTIL_DEF',
``.comm util_'ID`.$1, $2'
ifelse(`$#', `2', , `__UTIL_DEF(shift(shift($@)))')') 

/* Defines a data structure with multiple member fields, which are all
 * uninitialized (GNU as `.comm' directive).
 *
 * Parameters: 
 * `$1' - An identifier for this particular structure.
 * The other following arguments are always pairs of an identifier and the size
 * of the referenced memory location.
 *
 * Notes:
 * You can also just pass an identifier as `$1' and pass a macro defined
 * using `UTIL_STRUCT'. */
define(`UTIL_DEF',
`ifelse(eval(`$# < 1'), 1, `UTIL_FATAL(`identifier missing')')
ifelse(__UTIL_CHECK_STRUCT_ARGS(`2', shift($@)), `0', UTIL_FATAL(dnl
`data size missing'))
pushdef(`ID', `$1')
__UTIL_DEF(shift($@))
popdef(`ID')')

/* Defines an uninitialized structure. Technically, it just generates an
 * argument list, which can be passed to `UTIL_DEF'.
 *
 * Parameters:
 * `$1' - The name of the structure.
 * For the other arguments, this holds: 
 * The first argument is an identifier, the second the length of the
 * corresponding variable in bytes, and the list is continued with successive
 * pairs of identifiers and length values. In conclusion, `$#' must be a
 * multiple of `2', otherwise the macro exits with an error. */
define(`UTIL_STRUCT',
`ifelse(__UTIL_CHECK_STRUCT_ARGS(`2', shift($@)), `0', UTIL_FATAL(dnl
`data size missing'))
define(`$1', `shift($@)')')



/* Helper function for `UTIL_DEF_INIT'. Does the actual enumeration of data.
 *
 * Parameters:
 * `ID' - String inserted into the field definitions to keep them unique.
 * `$n' - Data fields to define. */
define(`__UTIL_DEF_INIT',
``util_'ID`.$1: .$2 $3'
ifelse(`$#', `3', , `__UTIL_DEF_INIT(shift(shift(shift($@)))')')

/* Defines a data structure with multiple member fields, which have all
 * a value they are initialized with (GNU as `.byte', `.int', etc. directives).
 *
 * Parameters:
 * `$1' - An identifier for this particular structure.
 * The other arguments form triples of an identifier for the data field,
 * a data type (such as `byte' or `int'), and a value to initialize the
 * field. */
define(`UTIL_DEF_INIT',
`ifelse(eval(`$# < 1'), 1, `UTIL_FATAL(`identifier missing')')
ifelse(__UTIL_CHECK_STRUCT_ARGS(`3', shift($@)), `1',
`pushdef(`ID', `$1')
__UTIL_DEF_INIT(shift($@))
popdef(`ID')',
`UTIL_FATAL(`data type and/or initialization value missing')')')
/* TODO: end of file in argument list. i guess there's some brace missing. */ 

/* Defines an initialized structure. Technically, it just generates an
 * argument list, which can be passed to `UTIL_DEF_INIT'.
 *
 * Parameters:
 * `$1' - The name of the structure.
 * For the other arguments, this holds:
 * The first argument is an identifier, the second the data type of the field,
 * the third the value of the field, and the list is continued with successive
 * triples of identifiers, data types, and initialization values. In conclusion,
 * `$#' must be a multiple of `3', otherwise the macro exits with an error. */
define(`UTIL_STRUCT_INIT',
`ifelse(__UTIL_CHECK_STRUCT_ARGS(`3', shift($@)), `1', `define(`$1',
`shift($@)')', `UTIL_FATAL(`data type and/or initialization value missing')')')

/* TODO: change the init versions so that the defstruct macro only describes
 * the names and sizes of the fields and the def macro initializes them
 * with values! currently defstruct takes over initialization, which
 * actually makes no sense. */

)
