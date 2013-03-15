#ifndef SERVERBASE_H
# define SERVERBASE_H


//////////////////////////////////////////////////////////////////////////
// Helper stuffs

typedef unsigned char       uchar;
typedef unsigned short      ushort;
typedef unsigned int        uint;

#define interface_class     class
#define abstract_class			class
#define static_class				class

# include <vector>
# include <list>
# include <hash_map>

// RakNet
# include <RakPeerInterface.h>

// Loggin & formats
# include <pantheios/pantheios.hpp> //primary header file, always be included

//# include <fastformat/fastformat.h>
# include <fastformat/fastformat.hpp>
# include <fastformat/sinks/ostream.hpp>
# include <fastformat/shims/conversion/filter_type/reals.hpp>
# include <fastformat/shims/conversion/filter_type/bool.hpp>

#if !defined(FASTFORMAT_NO_NAMESPACE)
namespace fastformat
{
  namespace filters
  {
#endif /* !FASTFORMAT_NO_NAMESPACE */

#if 1
    inline std::basic_string<ff_char_t> filter_type(char value, char const*, ff_char_t const volatile*)
    {
      return std::basic_string<ff_char_t>(1u, value);
    }

    inline std::basic_string<ff_char_t> filter_type(uchar value, uchar const*, ff_char_t const volatile*)
    {
      return std::basic_string<ff_char_t>(1u, value);
    }

    template<typename T>
    inline std::string filter_type(T const& o, T const*, char const volatile*)
    {
      std::stringstream result;
      result << o;
      return result.str();
    }

#else /* ? 0 */
    inline ff_string_slice_t filter_type(char const& value, char const*, ff_char_t const volatile*)
    {
      ff_string_slice_t   result;

      result.len  =   1;
      result.ptr  =   &value;

      return result;
    }
#endif /* 0 */

#if !defined(FASTFORMAT_NO_NAMESPACE)
  } /* namespace filters */
} /* namespace fastformat */
#endif /* !FASTFORMAT_NO_NAMESPACE */

namespace pan = pantheios;
namespace ff = fastformat;

namespace fastformat
{
  namespace sinks
  {
    template <int L>  
    pantheios::level<L>& fmt_slices(
        pantheios::level<L>&     sink
      , int                      flags
      , size_t                   cchTotal
      , size_t                   numResults
      , ff_string_slice_t const* results)
    {
      STLSOFT_STATIC_ASSERT(sizeof(
        pantheios::pan_slice_t) == 
        sizeof(fastformat::ff_string_slice_t));  
      STLSOFT_STATIC_ASSERT(offsetof(
        pantheios::pan_slice_t, len) == 
        offsetof(fastformat::ff_string_slice_t, 
        len));  

      pantheios::core::pantheios_log_n( 
          sink  
        , numResults  
        , stlsoft::sap_cast<pantheios::pan_slice_t const*>(results)  
        );  

      return sink;  
    }
  }
}

#  define log_CRITICAL      pantheios::log_CRITICAL
#  define log_NOTICE        pantheios::log_NOTICE
#  define log_WARNING       pantheios::log_WARNING
#  define log_ERROR         pantheios::log_ERROR

////////////////////////////////////////////////////

# include "ServerUtils.h"
# include "Opcodes.h"


////

// FastDelegates & Events
# include "Helpers/FastDelegate.h"
# include "Helpers/Event.h"

# include "Helpers/TagUserData.h"
# include "Helpers/threadpool.hpp"

using namespace fastdelegate;
using namespace DelegateEvent;


#endif // !SERVERBASE_H