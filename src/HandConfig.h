// HandConfig.h
#pragma once

// Select the hand through the PlatformIO environment's build_flags.

#if !defined(LEFT_HAND) && !defined(RIGHT_HAND)
  #error "Define exactly one: LEFT_HAND or RIGHT_HAND (build flag or uncomment in HandConfig.h)."
#endif

#if defined(LEFT_HAND) && defined(RIGHT_HAND)
  #error "Do not define both LEFT_HAND and RIGHT_HAND."
#endif
