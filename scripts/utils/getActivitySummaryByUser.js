/// @addtogroup scripts
/// @{
/// @file
/// @brief Given a specific `Range` from an "Activity" sheet, load the data from
/// the sheet in batches and summarize the usage by `MakerLabs ID`. Supports
/// optional set of filter values which each row must match to be included.

/// @brief Given a specific `Range` from an "Activity" sheet, load the data from
/// the sheet in batches and summarize the usage by `MakerLabs ID`. Supports
/// optional set of filter values which each row must match to be included.
///
/// @param range `Range` of "Activity" sheet rows to be summarized.
/// @param batchSize Number of rows to fetch in each batch, to avoid loading
/// entire sheet into memory.
/// @param filter (optional) Object with column titles as keys, and expected
/// values as values. Rows with any columns that do not match their filter
/// values are ignored.
///
/// @return Object containing activity summary by `MakerLabs ID`.
function getActivitySummaryByUser(range, batchSize, filter) {
  //const sheetName = "Activity";
  const sheetName = "Activity, De-duped";

  const spreadsheet = SpreadsheetApp.getActiveSpreadsheet();
  const sheet = spreadsheet.getSheetByName(sheetName);

  /// (Extract the header rows)
  const headersRange = sheet.getRange(1, 1, 1, sheet.getMaxColumns());
  const headersValues = headersRange.getValues();
  const headers = headersValues[0];

  const numRows = Number(range.getNumRows());
  const numColumns = Number(range.getNumColumns());

  const firstRow = Number(range.getRow());
  const lastRow = numRows + firstRow - 1;

  var activityByUser = {};
  const makerLabsIdColumn = headers.indexOf("MakerLabs ID");
  const machineIdColumn = headers.indexOf("Machine ID");
  const activityTypeColumn = headers.indexOf("Activity Type");
  const userColumns = {
    Name: headers.indexOf("Name"),
    Email: headers.indexOf("Email"),
    "Member's Billing Date": headers.indexOf("Member's Billing Date"),
    "Billing Date": headers.indexOf("Billing Date"),
    "Group Billing Week": headers.indexOf("Group Billing Week"),
    //"Tag Serial": headers.indexOf("Tag Serial"),
    //"MakerLabs ID": headers.indexOf("MakerLabs ID"),
  };
  const activityColumns = {
    "Maker Status": headers.indexOf("Maker Status"),
    //"Timestamp (UTC)": headers.indexOf("Timestamp (UTC)"),
    "Activity Type": headers.indexOf("Activity Type"),
    "Usage (seconds)": headers.indexOf("Usage (seconds)"),
    "Time (PST)": headers.indexOf("Time (PST)"),
    "Usage Email Sent Time": headers.indexOf("Usage Email Sent Time"),
    "Billing Email Sent Time": headers.indexOf("Billing Email Sent Time"),
  };

  const filterColumns = {};
  const filterColumnNames = Object.keys(filter);
  for (var idx = 0; idx < filterColumnNames.length; ++idx) {
    var colName = filterColumnNames[idx];
    filterColumns[colName] = headers.indexOf(colName);
  }

  /// (Verify the expected columns were found, otherwise exit early)
  if (
    makerLabsIdColumn > -1 &&
    machineIdColumn > -1 &&
    activityTypeColumn > -1 &&
    Object.keys(userColumns).every(function(k) {
      return userColumns[k] > -1;
    }) &&
    Object.keys(activityColumns).every(function(k) {
      return activityColumns[k] > -1;
    }) &&
    Object.keys(filterColumns).every(function(k) {
      return filterColumns[k] > -1;
    })
  ) {
    const userColumnNames = Object.keys(userColumns);
    const activityColumnNames = Object.keys(activityColumns);

    /// - Iterate through the rows in batches
    for (
      var currentRowIdx = 0;
      currentRowIdx < numRows;
      currentRowIdx += batchSize
    ) {
      var thisBatchSize = batchSize;

      /// - (Adjust size if outside bounds)
      if (currentRowIdx + thisBatchSize > numRows) {
        thisBatchSize = numRows - currentRowIdx;
      }

      var batchRange = range.offset(
        currentRowIdx,
        0,
        thisBatchSize,
        numColumns
      );
      var values = batchRange.getValues();

      /// - Iterate through the batch rows
      for (var row = 0; row < batchRange.getNumRows(); ++row) {
        var makerLabsId = values[row][makerLabsIdColumn];
        var activityType = values[row][activityTypeColumn];
        var rowNum = firstRow + currentRowIdx + row;

        /// - Only consider rows with a `MakerLabs ID` set
        if (makerLabsId) {
          var validRow = true;

          /// (If filter is set, ensure that all key/values have matching values
          /// in this row)
          if (filter) {
            for (var idx = 0; idx < filterColumnNames.length; ++idx) {
              var colName = filterColumnNames[idx];
              var col = filterColumns[colName];
              var value = values[row][col];

              if (
                typeof value.getTime === "function" &&
                typeof filter[colName].getTime === "function"
              ) {
                if (value.getTime() !== filter[colName].getTime()) {
                  validRow = false;
                  break;
                }
              } else if (value !== filter[colName]) {
                validRow = false;
                break;
              }
            }
          }

          if (validRow) {
            /// (If this is the first row for a user, populate the User columns
            /// that do not change)
            if (!(makerLabsId in activityByUser)) {
              activityByUser[makerLabsId] = {activity: {}};
              for (var idx = 0; idx < userColumnNames.length; ++idx) {
                var colName = userColumnNames[idx];
                var col = userColumns[colName];
                activityByUser[makerLabsId][colName] = values[row][col];
              }
            }

            /// (If this is the first activity for this machine ID, create an
            /// empty array)
            var machineId = values[row][machineIdColumn];
            if (!(machineId in activityByUser[makerLabsId].activity)) {
              activityByUser[makerLabsId].activity[machineId] = [];
            }

            /// - Start an empty activity array with the row for referencing
            /// later
            var activity = {row: rowNum};

            /// - Iterate through this row's columns
            for (var idx = 0; idx < activityColumnNames.length; ++idx) {
              var colName = activityColumnNames[idx];
              var col = activityColumns[colName];
              activity[colName] = values[row][col];
            }

            /// - Append the activity
            activityByUser[makerLabsId].activity[machineId].push(activity);
          }
        }
      }
    }
  }

  /// - Return the collected activity summaries.
  return activityByUser;
}
/// @}
