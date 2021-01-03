#pragma once

#include <stdbool.h>

#if __cplusplus
extern "C" {
#endif

void init_system_controls();

bool wait_for_events();

#if __cplusplus
}
#endif