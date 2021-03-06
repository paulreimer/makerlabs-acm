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
/// @brief Convert DataTable from result of `Users` query to `ACM.User`
/// flatbuffer.
import "dart:io" show HttpStatus;

import "dart:typed_data" show Uint8List;

// Local packages
import "gen/acm_a_c_m_generated.dart" as ACM;

import "http_response_exception.dart";
import "spreadsheet_helpers.dart";

const String ACM_FILE_IDENTIFIER = "ACM.";

/// @brief Convert DataTable from result of `Users` query to `ACM.User`
/// flatbuffer.
///
/// @param datatable Google Visualization DataTable
///
/// @return `ACM.User` flatbuffer bytes
Uint8List datatable_to_user(Map datatable) {
  if (datatable == null ||
      !datatable.containsKey("table") ||
      !datatable["table"].containsKey("rows")) {
    throw new HttpResponseException(
        "Invalid Google GViz Datatable in query response",
        statusCode: HttpStatus.badGateway);
  }

  if (!datatable["table"]["rows"].isEmpty) {
    final userRow = datatable["table"]["rows"][0]["c"];

    /// Expect query result columns in a hard-coded order as follows:
    /// -# name
    final String name = (userRow[1] != null)
        ? userRow[1]["v"]
        : (userRow[0] != null) ? userRow[0]["v"] : null;

    /// -# email
    final String email = (userRow[2] != null) ? userRow[2]["v"] : null;

    /// -# makerlabs_id
    final String makerlabs_id = (userRow[3] != null) ? userRow[3]["v"] : null;

    /// -# maker_status
    final String maker_status = (userRow[4] != null) ? userRow[4]["v"] : null;

    /// -# alerts
    final String alerts = (userRow[5] != null) ? userRow[5]["v"] : null;

    /// -# tagId
    final String tagId = (userRow[6] != null) ? userRow[6]["v"] : null;

    final List<String> permissions = [];
    for (int i = 7; i < userRow.length; ++i) {
      String permissionValue = (userRow[i] != null) ? userRow[i]["v"] : null;
      if (isYesLike(permissionValue)) {
        String permissionLabel = nthColumnLabel(datatable, i);
        permissions.add(permissionLabel);
      }
    }

    final userBuilder = new ACM.UserObjectBuilder(
      name: name,
      email: email,
      makerlabsId: makerlabs_id,
      makerStatus: maker_status,
      tagId: tagId,
      alerts: alerts,
      permissions: permissions,
    );

    return userBuilder.toBytes(ACM_FILE_IDENTIFIER);
  } else {
    print("No matching User for Activity tag_id");
  }

  return null;
}

/// @}
