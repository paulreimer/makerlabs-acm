function excludeTemplateRowFromRange(fullRange) {
  // Extract sheet which holds the provided range
  const sheet = fullRange.getSheet();

  // Extract the row which will serve as the template
  const templateRowRange = getTemplateRowForSheet(sheet.getName());

  // Determine the last row in the template range
  const templateRow = Number(templateRowRange.getLastRow());

  // Determine the first row in the range
  const firstRangeRow = Number(fullRange.getRow());

  // Check if the range includes any headers rows
  if (firstRangeRow < templateRow) {
    // Determine how many rows to offset to get past the headers
    const rowsToOffset = templateRow - firstRangeRow;

    // Remove an according number of rows from the length of the range
    const numRowsAfterOffset = fullRange.getNumRows() - rowsToOffset;

    // Figure out column offsets that select all columns
    const firstColumnOffset = 0;
    const numColumns = Number(fullRange.getNumColumns());

    // Offset the range and return the resulting range
    const excludedRange = fullRange.offset(
      rowsToOffset,
      firstColumnOffset,
      numRowsAfterOffset,
      numColumns
    );

    return excludedRange;
  }

  // Return the input directly, if it didn't include any header rows
  return fullRange;
}