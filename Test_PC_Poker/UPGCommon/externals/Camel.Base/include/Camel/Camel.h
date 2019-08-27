#ifndef __Camel_Generic__
#define __Camel_Generic__

// Common
#include "Common/Filter.h"
#include "Common/FilterChain.h"
#include "Common/IOHandler.h"
#include "Common/ThreadPool.h"


// Filter
#include "Filter/AutoRecvFilter.h"

// Transport
#include "Transport/Connector.h"
#include "Transport/Link.h"
#include "Transport/LinkTraits.h"
#include "Transport/DefaultLinkFactory.h"
#include "Transport/InputStream.h"
#include "Transport/OutputStream.h"

#endif