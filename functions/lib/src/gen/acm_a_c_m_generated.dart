// automatically generated by the FlatBuffers compiler, do not modify
// ignore_for_file: unused_import, unused_field, unused_local_variable

library a_c_m;

import 'dart:typed_data' show Uint8List;
import 'package:flat_buffers/flat_buffers.dart' as fb;


class ActivityType {
  final int value;
  const ActivityType._(this.value);

  factory ActivityType.fromValue(int value) {
    if (value == null) value = 0;
    if (!values.containsKey(value)) {
      throw new StateError('Invalid value $value for bit flag enum ActivityType');
    }
    return values[value];
  }

  static const int minValue = 0;
  static const int maxValue = 2;
  static bool containsValue(int value) => values.containsKey(value);

  static const ActivityType Signed_In = const ActivityType._(0);
  static const ActivityType Signed_Out = const ActivityType._(1);
  static const ActivityType CNC_Job = const ActivityType._(2);
  static get values => {0: Signed_In,1: Signed_Out,2: CNC_Job,};

  static const fb.Reader<ActivityType> reader = const _ActivityTypeReader();

  @override
  String toString() {
    return 'ActivityType{value: $value}';
  }
}

class _ActivityTypeReader extends fb.Reader<ActivityType> {
  const _ActivityTypeReader();

  @override
  int get size => 1;

  @override
  ActivityType read(fb.BufferContext bc, int offset) =>
      new ActivityType.fromValue(const fb.Int8Reader().read(bc, offset));
}

class LogSeverity {
  final int value;
  const LogSeverity._(this.value);

  factory LogSeverity.fromValue(int value) {
    if (value == null) value = 0;
    if (!values.containsKey(value)) {
      throw new StateError('Invalid value $value for bit flag enum LogSeverity');
    }
    return values[value];
  }

  static const int minValue = 0;
  static const int maxValue = 6;
  static bool containsValue(int value) => values.containsKey(value);

  static const LogSeverity Fatal = const LogSeverity._(0);
  static const LogSeverity Error = const LogSeverity._(1);
  static const LogSeverity Warn = const LogSeverity._(2);
  static const LogSeverity Notice = const LogSeverity._(3);
  static const LogSeverity Info = const LogSeverity._(4);
  static const LogSeverity Debug = const LogSeverity._(5);
  static const LogSeverity Trace = const LogSeverity._(6);
  static get values => {0: Fatal,1: Error,2: Warn,3: Notice,4: Info,5: Debug,6: Trace,};

  static const fb.Reader<LogSeverity> reader = const _LogSeverityReader();

  @override
  String toString() {
    return 'LogSeverity{value: $value}';
  }
}

class _LogSeverityReader extends fb.Reader<LogSeverity> {
  const _LogSeverityReader();

  @override
  int get size => 1;

  @override
  LogSeverity read(fb.BufferContext bc, int offset) =>
      new LogSeverity.fromValue(const fb.Int8Reader().read(bc, offset));
}

class Activity {
  Activity._(this._bc, this._bcOffset);
  factory Activity(List<int> bytes) {
    fb.BufferContext rootRef = new fb.BufferContext.fromBytes(bytes);
    return reader.read(rootRef, 0);
  }

  static const fb.Reader<Activity> reader = const _ActivityReader();

  final fb.BufferContext _bc;
  final int _bcOffset;

  double get time => const fb.Float64Reader().vTableGet(_bc, _bcOffset, 4, 0.0);
  String get machineId => const fb.StringReader().vTableGet(_bc, _bcOffset, 6, null);
  ActivityType get activityType => new ActivityType.fromValue(const fb.Int8Reader().vTableGet(_bc, _bcOffset, 8, null));
  String get tagId => const fb.StringReader().vTableGet(_bc, _bcOffset, 10, null);
  int get usageSeconds => const fb.Uint32Reader().vTableGet(_bc, _bcOffset, 12, null);

  @override
  String toString() {
    return 'Activity{time: $time, machineId: $machineId, activityType: $activityType, tagId: $tagId, usageSeconds: $usageSeconds}';
  }
}

class _ActivityReader extends fb.TableReader<Activity> {
  const _ActivityReader();

  @override
  Activity createObject(fb.BufferContext bc, int offset) => 
    new Activity._(bc, offset);
}

class ActivityBuilder {
  ActivityBuilder(this.fbBuilder) {
    assert(fbBuilder != null);
  }

  final fb.Builder fbBuilder;

  void begin() {
    fbBuilder.startTable();
  }

  int addTime(double time) {
    fbBuilder.addFloat64(0, time);
    return fbBuilder.offset;
  }
  int addMachineIdOffset(int offset) {
    fbBuilder.addOffset(1, offset);
    return fbBuilder.offset;
  }
  int addActivityType(ActivityType activityType) {
    fbBuilder.addInt8(2, activityType?.value);
    return fbBuilder.offset;
  }
  int addTagIdOffset(int offset) {
    fbBuilder.addOffset(3, offset);
    return fbBuilder.offset;
  }
  int addUsageSeconds(int usageSeconds) {
    fbBuilder.addUint32(4, usageSeconds);
    return fbBuilder.offset;
  }

  int finish() {
    return fbBuilder.endTable();
  }
}

class ActivityObjectBuilder extends fb.ObjectBuilder {
  final double _time;
  final String _machineId;
  final ActivityType _activityType;
  final String _tagId;
  final int _usageSeconds;

  ActivityObjectBuilder({
    double time,
    String machineId,
    ActivityType activityType,
    String tagId,
    int usageSeconds,
  })
      : _time = time,
        _machineId = machineId,
        _activityType = activityType,
        _tagId = tagId,
        _usageSeconds = usageSeconds;

  /// Finish building, and store into the [fbBuilder].
  @override
  int finish(
    fb.Builder fbBuilder) {
    assert(fbBuilder != null);
    final int machineIdOffset = fbBuilder.writeString(_machineId);
    final int tagIdOffset = fbBuilder.writeString(_tagId);

    fbBuilder.startTable();
    fbBuilder.addFloat64(0, _time);
    if (machineIdOffset != null) {
      fbBuilder.addOffset(1, machineIdOffset);
    }
    fbBuilder.addInt8(2, _activityType?.value);
    if (tagIdOffset != null) {
      fbBuilder.addOffset(3, tagIdOffset);
    }
    fbBuilder.addUint32(4, _usageSeconds);
    return fbBuilder.endTable();
  }

  /// Convenience method to serialize to byte list.
  @override
  Uint8List toBytes([String fileIdentifier]) {
    fb.Builder fbBuilder = new fb.Builder();
    int offset = finish(fbBuilder);
    return fbBuilder.finish(offset, fileIdentifier);
  }
}
class CNC_Job {
  CNC_Job._(this._bc, this._bcOffset);
  factory CNC_Job(List<int> bytes) {
    fb.BufferContext rootRef = new fb.BufferContext.fromBytes(bytes);
    return reader.read(rootRef, 0);
  }

  static const fb.Reader<CNC_Job> reader = const _CNC_JobReader();

  final fb.BufferContext _bc;
  final int _bcOffset;

  int get usageSeconds => const fb.Uint32Reader().vTableGet(_bc, _bcOffset, 4, null);

  @override
  String toString() {
    return 'CNC_Job{usageSeconds: $usageSeconds}';
  }
}

class _CNC_JobReader extends fb.TableReader<CNC_Job> {
  const _CNC_JobReader();

  @override
  CNC_Job createObject(fb.BufferContext bc, int offset) => 
    new CNC_Job._(bc, offset);
}

class CNC_JobBuilder {
  CNC_JobBuilder(this.fbBuilder) {
    assert(fbBuilder != null);
  }

  final fb.Builder fbBuilder;

  void begin() {
    fbBuilder.startTable();
  }

  int addUsageSeconds(int usageSeconds) {
    fbBuilder.addUint32(0, usageSeconds);
    return fbBuilder.offset;
  }

  int finish() {
    return fbBuilder.endTable();
  }
}

class CNC_JobObjectBuilder extends fb.ObjectBuilder {
  final int _usageSeconds;

  CNC_JobObjectBuilder({
    int usageSeconds,
  })
      : _usageSeconds = usageSeconds;

  /// Finish building, and store into the [fbBuilder].
  @override
  int finish(
    fb.Builder fbBuilder) {
    assert(fbBuilder != null);

    fbBuilder.startTable();
    fbBuilder.addUint32(0, _usageSeconds);
    return fbBuilder.endTable();
  }

  /// Convenience method to serialize to byte list.
  @override
  Uint8List toBytes([String fileIdentifier]) {
    fb.Builder fbBuilder = new fb.Builder();
    int offset = finish(fbBuilder);
    return fbBuilder.finish(offset, fileIdentifier);
  }
}
class User {
  User._(this._bc, this._bcOffset);
  factory User(List<int> bytes) {
    fb.BufferContext rootRef = new fb.BufferContext.fromBytes(bytes);
    return reader.read(rootRef, 0);
  }

  static const fb.Reader<User> reader = const _UserReader();

  final fb.BufferContext _bc;
  final int _bcOffset;

  String get name => const fb.StringReader().vTableGet(_bc, _bcOffset, 4, null);
  String get email => const fb.StringReader().vTableGet(_bc, _bcOffset, 6, null);
  String get makerlabsId => const fb.StringReader().vTableGet(_bc, _bcOffset, 8, null);
  String get makerStatus => const fb.StringReader().vTableGet(_bc, _bcOffset, 10, null);
  String get tagId => const fb.StringReader().vTableGet(_bc, _bcOffset, 12, null);
  String get alerts => const fb.StringReader().vTableGet(_bc, _bcOffset, 14, null);
  List<String> get permissions => const fb.ListReader<String>(const fb.StringReader()).vTableGet(_bc, _bcOffset, 16, null);

  @override
  String toString() {
    return 'User{name: $name, email: $email, makerlabsId: $makerlabsId, makerStatus: $makerStatus, tagId: $tagId, alerts: $alerts, permissions: $permissions}';
  }
}

class _UserReader extends fb.TableReader<User> {
  const _UserReader();

  @override
  User createObject(fb.BufferContext bc, int offset) => 
    new User._(bc, offset);
}

class UserBuilder {
  UserBuilder(this.fbBuilder) {
    assert(fbBuilder != null);
  }

  final fb.Builder fbBuilder;

  void begin() {
    fbBuilder.startTable();
  }

  int addNameOffset(int offset) {
    fbBuilder.addOffset(0, offset);
    return fbBuilder.offset;
  }
  int addEmailOffset(int offset) {
    fbBuilder.addOffset(1, offset);
    return fbBuilder.offset;
  }
  int addMakerlabsIdOffset(int offset) {
    fbBuilder.addOffset(2, offset);
    return fbBuilder.offset;
  }
  int addMakerStatusOffset(int offset) {
    fbBuilder.addOffset(3, offset);
    return fbBuilder.offset;
  }
  int addTagIdOffset(int offset) {
    fbBuilder.addOffset(4, offset);
    return fbBuilder.offset;
  }
  int addAlertsOffset(int offset) {
    fbBuilder.addOffset(5, offset);
    return fbBuilder.offset;
  }
  int addPermissionsOffset(int offset) {
    fbBuilder.addOffset(6, offset);
    return fbBuilder.offset;
  }

  int finish() {
    return fbBuilder.endTable();
  }
}

class UserObjectBuilder extends fb.ObjectBuilder {
  final String _name;
  final String _email;
  final String _makerlabsId;
  final String _makerStatus;
  final String _tagId;
  final String _alerts;
  final List<String> _permissions;

  UserObjectBuilder({
    String name,
    String email,
    String makerlabsId,
    String makerStatus,
    String tagId,
    String alerts,
    List<String> permissions,
  })
      : _name = name,
        _email = email,
        _makerlabsId = makerlabsId,
        _makerStatus = makerStatus,
        _tagId = tagId,
        _alerts = alerts,
        _permissions = permissions;

  /// Finish building, and store into the [fbBuilder].
  @override
  int finish(
    fb.Builder fbBuilder) {
    assert(fbBuilder != null);
    final int nameOffset = fbBuilder.writeString(_name);
    final int emailOffset = fbBuilder.writeString(_email);
    final int makerlabsIdOffset = fbBuilder.writeString(_makerlabsId);
    final int makerStatusOffset = fbBuilder.writeString(_makerStatus);
    final int tagIdOffset = fbBuilder.writeString(_tagId);
    final int alertsOffset = fbBuilder.writeString(_alerts);
    final int permissionsOffset = _permissions?.isNotEmpty == true
        ? fbBuilder.writeList(_permissions.map((b) => fbBuilder.writeString(b)).toList())
        : null;

    fbBuilder.startTable();
    if (nameOffset != null) {
      fbBuilder.addOffset(0, nameOffset);
    }
    if (emailOffset != null) {
      fbBuilder.addOffset(1, emailOffset);
    }
    if (makerlabsIdOffset != null) {
      fbBuilder.addOffset(2, makerlabsIdOffset);
    }
    if (makerStatusOffset != null) {
      fbBuilder.addOffset(3, makerStatusOffset);
    }
    if (tagIdOffset != null) {
      fbBuilder.addOffset(4, tagIdOffset);
    }
    if (alertsOffset != null) {
      fbBuilder.addOffset(5, alertsOffset);
    }
    if (permissionsOffset != null) {
      fbBuilder.addOffset(6, permissionsOffset);
    }
    return fbBuilder.endTable();
  }

  /// Convenience method to serialize to byte list.
  @override
  Uint8List toBytes([String fileIdentifier]) {
    fb.Builder fbBuilder = new fb.Builder();
    int offset = finish(fbBuilder);
    return fbBuilder.finish(offset, fileIdentifier);
  }
}
class Log {
  Log._(this._bc, this._bcOffset);
  factory Log(List<int> bytes) {
    fb.BufferContext rootRef = new fb.BufferContext.fromBytes(bytes);
    return reader.read(rootRef, 0);
  }

  static const fb.Reader<Log> reader = const _LogReader();

  final fb.BufferContext _bc;
  final int _bcOffset;

  String get machineId => const fb.StringReader().vTableGet(_bc, _bcOffset, 4, null);
  LogSeverity get severity => new LogSeverity.fromValue(const fb.Int8Reader().vTableGet(_bc, _bcOffset, 6, null));
  int get time => const fb.Uint64Reader().vTableGet(_bc, _bcOffset, 8, null);
  String get message => const fb.StringReader().vTableGet(_bc, _bcOffset, 10, null);

  @override
  String toString() {
    return 'Log{machineId: $machineId, severity: $severity, time: $time, message: $message}';
  }
}

class _LogReader extends fb.TableReader<Log> {
  const _LogReader();

  @override
  Log createObject(fb.BufferContext bc, int offset) => 
    new Log._(bc, offset);
}

class LogBuilder {
  LogBuilder(this.fbBuilder) {
    assert(fbBuilder != null);
  }

  final fb.Builder fbBuilder;

  void begin() {
    fbBuilder.startTable();
  }

  int addMachineIdOffset(int offset) {
    fbBuilder.addOffset(0, offset);
    return fbBuilder.offset;
  }
  int addSeverity(LogSeverity severity) {
    fbBuilder.addInt8(1, severity?.value);
    return fbBuilder.offset;
  }
  int addTime(int time) {
    fbBuilder.addUint64(2, time);
    return fbBuilder.offset;
  }
  int addMessageOffset(int offset) {
    fbBuilder.addOffset(3, offset);
    return fbBuilder.offset;
  }

  int finish() {
    return fbBuilder.endTable();
  }
}

class LogObjectBuilder extends fb.ObjectBuilder {
  final String _machineId;
  final LogSeverity _severity;
  final int _time;
  final String _message;

  LogObjectBuilder({
    String machineId,
    LogSeverity severity,
    int time,
    String message,
  })
      : _machineId = machineId,
        _severity = severity,
        _time = time,
        _message = message;

  /// Finish building, and store into the [fbBuilder].
  @override
  int finish(
    fb.Builder fbBuilder) {
    assert(fbBuilder != null);
    final int machineIdOffset = fbBuilder.writeString(_machineId);
    final int messageOffset = fbBuilder.writeString(_message);

    fbBuilder.startTable();
    if (machineIdOffset != null) {
      fbBuilder.addOffset(0, machineIdOffset);
    }
    fbBuilder.addInt8(1, _severity?.value);
    fbBuilder.addUint64(2, _time);
    if (messageOffset != null) {
      fbBuilder.addOffset(3, messageOffset);
    }
    return fbBuilder.endTable();
  }

  /// Convenience method to serialize to byte list.
  @override
  Uint8List toBytes([String fileIdentifier]) {
    fb.Builder fbBuilder = new fb.Builder();
    int offset = finish(fbBuilder);
    return fbBuilder.finish(offset, fileIdentifier);
  }
}
