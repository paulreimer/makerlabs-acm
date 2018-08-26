// Copyright Paul Reimer, 2018
//
// This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 Unported License.
// To view a copy of this license, visit
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// or send a letter to
// Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.

import "dart:async";
import "dart:core";
import "dart:convert";
import "package:node_io/node_io.dart" show Platform, HttpStatus;
import "dart:typed_data" show Uint8List;

// Dart / Firebase Functions interop
import 'package:firebase_functions_interop/firebase_functions_interop.dart';

// Dart / NodeJS interop
import "package:node_http/node_http.dart" as http;
import "package:node_interop/buffer.dart";

// Google Cloud Functions interop
import "google_cloud_functions.dart";

// Local packages
import "src/gen/acm_a_c_m_generated.dart" as ACM;

import "src/append_activity_to_spreadsheet.dart";
import "src/datatable_to_user.dart";
import "src/extract_access_token.dart";
import "src/fetch_sheet_columns.dart";
import "src/generate_permissions_check_query.dart";
import "src/http_response_exception.dart";
import "src/push_latest_user_to_firebase.dart";
import "src/spreadsheet_client.dart";

final String SPREADSHEET_QUERY_AUTHORITY = "docs.google.com";

// Users permissions check query:
const String SPREADSHEET_USERS_SHEET_NAME = "Users";

// Activity row append:
const String SPREADSHEET_ACTIVITY_SHEET_NAME = "Activity";

// This global value will persist across function invocations
String USER_COLUMNS_DATATABLE;

http.NodeClient HTTP_CLIENT;

FirebaseAdmin FIREBASE_admin;
App FIREBASE_app;
Database FIREBASE_database;

// Firebase Function HTTPS handler
Future<void> permissions_check_http(GoogleCloudFunctionsRequest request,
    GoogleCloudFunctionsResponse response) async {
  // Early exit for external ping check to keep function "warm"
  final uri = Uri.parse(request.url);
  if (uri.queryParameters.containsKey("ping")) {
    print("ping");
    response
      ..statusCode = HttpStatus.ok
      ..end();
    return;
  }

  // Return 500 error for any unhandled exceptions
  try {
    if (HTTP_CLIENT == null) {
      print("Initialize HTTP_CLIENT");
      HTTP_CLIENT = new http.NodeClient(keepAlive: true);
    } else {
      print("Use cached HTTP_CLIENT");
    }

    // Google Sheets ACM Spreadsheet ID
    String SPREADSHEET_ID;

    Map<String, String> envVars = Platform.environment;

    final config = FirebaseFunctions.config;
    const String firebase_spreadsheet_id_key = "acm.spreadsheet_id";
    const String env_var = "SPREADSHEET_ID";
    if (config?.get(firebase_spreadsheet_id_key) != null) {
      SPREADSHEET_ID = config.get(firebase_spreadsheet_id_key);
    } else {
      SPREADSHEET_ID = envVars[env_var];
    }

    if (SPREADSHEET_ID == null) {
      throw ("Missing env var: SPREADSHEET_ID");
    }

    final String spreadsheetQueryPath =
        "/spreadsheets/d/" + SPREADSHEET_ID + "/gviz/tq";

    // Parse request headers
    String authorization;
    for (int i = 0; i < (request.rawHeaders.length - 1); ++i) {
      if (request.rawHeaders[i] == "Authorization" ||
          request.rawHeaders[i] == "authorization") {
        authorization = request.rawHeaders[i + 1];
      }
    }

    // Extract OAuth access_token from request, use it for Google API requests
    String access_token = extract_access_token(authorization);

    // Parse request body
    // Expect a binary ACM.Activity flatbuffer
    Uint8List bytes = request.body;

    final activity = new ACM.Activity(bytes);

    // Check that the Activity flatbuffer has the expected fields
    if (activity.time == null ||
        activity.tagId == null ||
        activity.activityType == null ||
        activity.machineId == null) {
      throw new HttpResponseException(
          "Invalid (empty) Activity in request body",
          statusCode: HttpStatus.badRequest);
    }

    // Firebase RTDB Activity ref
    if (FIREBASE_admin == null) {
      FIREBASE_admin = FirebaseAdmin.instance;
    }
    if (FIREBASE_app == null) {
      // If environment variables for Firebase service account are provided, use them
      if (envVars.containsKey("FIREBASE_SERVICE_ACCOUNT_PRIVATE_KEY")) {
        FIREBASE_app = FIREBASE_admin.initializeApp(new AppOptions(
            credential: FIREBASE_admin.cert(
                projectId: envVars["GCLOUD_PROJECT"],
                clientEmail: envVars["FIREBASE_SERVICE_ACCOUNT_CLIENT_EMAIL"],
                privateKey: envVars["FIREBASE_SERVICE_ACCOUNT_PRIVATE_KEY"]),
            databaseURL: envVars["FIREBASE_DATABASE_URL"]));
      } else {
        // Use Application Default Credentials in Google Cloud
        FIREBASE_app = FIREBASE_admin.initializeApp();
      }
    }
    if (FIREBASE_database == null) {
      FIREBASE_database = FIREBASE_app.database();
    }

    final Reference activityRef =
        FIREBASE_database.ref("/activity/${activity.machineId}");
    final Reference latestUserRef =
        FIREBASE_database.ref("readers/${activity.machineId}/latestUser");

    if (activityRef != null) {
      //push_activity_to_firebase(activityRef, activity);
    }

    // Extract the latest Sheets API from the generated APIs
    final sheets = spreadsheet_client(access_token, HTTP_CLIENT);

    // Check for cached User sheet columns, fetch them if not present
    if (USER_COLUMNS_DATATABLE == null) {
      USER_COLUMNS_DATATABLE = await fetch_sheet_columns(
          SPREADSHEET_QUERY_AUTHORITY,
          spreadsheetQueryPath,
          SPREADSHEET_USERS_SHEET_NAME,
          access_token);
      print("Did update USER_COLUMNS_DATATABLE via spreadsheet query");
    } else {
      print("Use cached USER_COLUMNS_DATATABLE");
    }

    final Map usersColumnsDatatable = json.decode(USER_COLUMNS_DATATABLE);

    final List<List<String>> selectColumnLabels = [
      ["Maker Info", "Name"],
      ["Maker Info", "Display Name"],
      ["Maker Info", "Email"],
      ["Membership Info", "MakerLabs ID"],
      ["Membership Info", "Maker Status"],
      ["Membership Info", "Alerts"],
      ["Access & Studio", "Tag ID"]
    ];

    String query = generate_permissions_check_query(
        usersColumnsDatatable, selectColumnLabels, activity.tagId);

    final uri = Uri.https(SPREADSHEET_QUERY_AUTHORITY, spreadsheetQueryPath, {
      "sheet": SPREADSHEET_USERS_SHEET_NAME,
      "tq": query,
      "access_token": access_token,
    });
    final queryHeaders = {
      "X-DataSource-Auth": "force-json-workaround",
    };

    final queryResponse = await HTTP_CLIENT.get(uri, headers: queryHeaders);

    // Check for valid query response
    if (queryResponse.statusCode != HttpStatus.ok) {
      throw new HttpResponseException("Google Query operation failed",
          statusCode: queryResponse.statusCode);
    }

    // We will be sending a 200 response with a matching User (or empty)
    ACM.User user;
    response
      ..statusCode = HttpStatus.ok
      ..setHeader("Content-Type", "application/octet-stream");

    String datatableJson = queryResponse.body;
    // Check for a valid response, which may or may not contain a user
    // Remove "" prefix from response JSON
    const String googleMagic = ")]}'\n";
    if (datatableJson.startsWith(googleMagic)) {
      datatableJson = datatableJson.substring(googleMagic.length);
    }

    final Map datatable = json.decode(datatableJson);

    final userBytes = datatable_to_user(datatable);
    if (userBytes != null) {
      user = new ACM.User(userBytes);

      if (user != null) {
        print("got User: ${user}");

        if (latestUserRef != null) {
          push_latest_user_to_firebase(latestUserRef, user);
        }
        response.write(Buffer.from(userBytes));
      }
    }

    response.end();

    await append_activity_to_spreadsheet(sheets, SPREADSHEET_ID,
        SPREADSHEET_ACTIVITY_SHEET_NAME, activity, user);
  } catch (e) {
    // In case of general failure, return response with exception text
    print("Trapped exception: ${e.toString()}");

    // If a specific error code has not been set, send a general error
    response.statusCode = (e is HttpResponseException)
        ? e.statusCode
        : HttpStatus.internalServerError;

    response
      ..write(e.toString())
      ..end();
  }
}