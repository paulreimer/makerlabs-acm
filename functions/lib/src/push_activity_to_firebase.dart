// Copyright Paul Reimer, 2018
//
// This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 Unported License.
// To view a copy of this license, visit
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// or send a letter to
// Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.

/// @addtogroup functions
/// @{
/// @file
/// @brief Append Activity details to Firebase to provided Database Reference

// Dart / Firebase Functions interop
import 'package:firebase_functions_interop/firebase_functions_interop.dart';

// Local packages
import "gen/acm_a_c_m_generated.dart" as ACM;

/// @brief Append Activity details to Firebase to provided Database Reference
///
/// @param activityRef Firebase Database Reference
/// (e.g. `readers/{machineId}/activity`)
/// @param activity `ACM.Activity` flatbuffer
///
/// @return Future that completes when write to Firebase Database is done.
FutureReference push_activity_to_firebase(
    Reference activityRef, ACM.Activity activity) {
  final Map activityMap = {
    "time": activity.time,
    "machineId": activity.machineId,
    "activityType": activity.activityType.value,
    "tagId": activity.tagId,
    "usageSeconds": activity.usageSeconds,
  };

  return activityRef.push(activityMap);
}

/// @}
