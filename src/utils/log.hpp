
#pragma once
#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/std/Array.h"
#include "quill/std/Vector.h"
#include "quill/std/UnorderedMap.h"
#include "quill/std/UnorderedSet.h"
#include "quill/std/Tuple.h"
#include "quill/std/FilesystemPath.h"
#include "ConsoleSinkWithFormatter.h"
#include "quill/sinks/RotatingFileSink.h"

#include "mytime.hpp"

extern void quill_init();
inline quill::Logger *logger;