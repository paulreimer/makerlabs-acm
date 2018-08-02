// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ACM_ACM_H_
#define FLATBUFFERS_GENERATED_ACM_ACM_H_

#include "flatbuffers/flatbuffers.h"

namespace ACM {

struct Activity;

struct User;

struct Log;

inline const flatbuffers::TypeTable *ActivityTypeTable();

inline const flatbuffers::TypeTable *UserTypeTable();

inline const flatbuffers::TypeTable *LogTypeTable();

enum class ActivityType : int8_t {
  Signed_In = 0,
  Signed_Out = 1,
  CNC_Job = 2,
  MIN = Signed_In,
  MAX = CNC_Job
};

inline const ActivityType (&EnumValuesActivityType())[3] {
  static const ActivityType values[] = {
    ActivityType::Signed_In,
    ActivityType::Signed_Out,
    ActivityType::CNC_Job
  };
  return values;
}

inline const char * const *EnumNamesActivityType() {
  static const char * const names[] = {
    "Signed_In",
    "Signed_Out",
    "CNC_Job",
    nullptr
  };
  return names;
}

inline const char *EnumNameActivityType(ActivityType e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesActivityType()[index];
}

enum class LogSeverity : int8_t {
  Fatal = 0,
  Error = 1,
  Warn = 2,
  Notice = 3,
  Info = 4,
  Debug = 5,
  Trace = 6,
  MIN = Fatal,
  MAX = Trace
};

inline const LogSeverity (&EnumValuesLogSeverity())[7] {
  static const LogSeverity values[] = {
    LogSeverity::Fatal,
    LogSeverity::Error,
    LogSeverity::Warn,
    LogSeverity::Notice,
    LogSeverity::Info,
    LogSeverity::Debug,
    LogSeverity::Trace
  };
  return values;
}

inline const char * const *EnumNamesLogSeverity() {
  static const char * const names[] = {
    "Fatal",
    "Error",
    "Warn",
    "Notice",
    "Info",
    "Debug",
    "Trace",
    nullptr
  };
  return names;
}

inline const char *EnumNameLogSeverity(LogSeverity e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesLogSeverity()[index];
}

struct Activity FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return ActivityTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "ACM.Activity";
  }
  enum {
    VT_TIME = 4,
    VT_MACHINE_ID = 6,
    VT_ACTIVITY_TYPE = 8,
    VT_USAGE_SECONDS = 10,
    VT_TAG_ID = 12
  };
  double time() const {
    return GetField<double>(VT_TIME, 0.0);
  }
  bool mutate_time(double _time) {
    return SetField<double>(VT_TIME, _time, 0.0);
  }
  const flatbuffers::String *machine_id() const {
    return GetPointer<const flatbuffers::String *>(VT_MACHINE_ID);
  }
  flatbuffers::String *mutable_machine_id() {
    return GetPointer<flatbuffers::String *>(VT_MACHINE_ID);
  }
  ActivityType activity_type() const {
    return static_cast<ActivityType>(GetField<int8_t>(VT_ACTIVITY_TYPE, 0));
  }
  bool mutate_activity_type(ActivityType _activity_type) {
    return SetField<int8_t>(VT_ACTIVITY_TYPE, static_cast<int8_t>(_activity_type), 0);
  }
  uint32_t usage_seconds() const {
    return GetField<uint32_t>(VT_USAGE_SECONDS, 0);
  }
  bool mutate_usage_seconds(uint32_t _usage_seconds) {
    return SetField<uint32_t>(VT_USAGE_SECONDS, _usage_seconds, 0);
  }
  const flatbuffers::String *tag_id() const {
    return GetPointer<const flatbuffers::String *>(VT_TAG_ID);
  }
  flatbuffers::String *mutable_tag_id() {
    return GetPointer<flatbuffers::String *>(VT_TAG_ID);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<double>(verifier, VT_TIME) &&
           VerifyOffset(verifier, VT_MACHINE_ID) &&
           verifier.Verify(machine_id()) &&
           VerifyField<int8_t>(verifier, VT_ACTIVITY_TYPE) &&
           VerifyField<uint32_t>(verifier, VT_USAGE_SECONDS) &&
           VerifyOffset(verifier, VT_TAG_ID) &&
           verifier.Verify(tag_id()) &&
           verifier.EndTable();
  }
};

struct ActivityBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_time(double time) {
    fbb_.AddElement<double>(Activity::VT_TIME, time, 0.0);
  }
  void add_machine_id(flatbuffers::Offset<flatbuffers::String> machine_id) {
    fbb_.AddOffset(Activity::VT_MACHINE_ID, machine_id);
  }
  void add_activity_type(ActivityType activity_type) {
    fbb_.AddElement<int8_t>(Activity::VT_ACTIVITY_TYPE, static_cast<int8_t>(activity_type), 0);
  }
  void add_usage_seconds(uint32_t usage_seconds) {
    fbb_.AddElement<uint32_t>(Activity::VT_USAGE_SECONDS, usage_seconds, 0);
  }
  void add_tag_id(flatbuffers::Offset<flatbuffers::String> tag_id) {
    fbb_.AddOffset(Activity::VT_TAG_ID, tag_id);
  }
  explicit ActivityBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ActivityBuilder &operator=(const ActivityBuilder &);
  flatbuffers::Offset<Activity> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Activity>(end);
    return o;
  }
};

inline flatbuffers::Offset<Activity> CreateActivity(
    flatbuffers::FlatBufferBuilder &_fbb,
    double time = 0.0,
    flatbuffers::Offset<flatbuffers::String> machine_id = 0,
    ActivityType activity_type = ActivityType::Signed_In,
    uint32_t usage_seconds = 0,
    flatbuffers::Offset<flatbuffers::String> tag_id = 0) {
  ActivityBuilder builder_(_fbb);
  builder_.add_time(time);
  builder_.add_tag_id(tag_id);
  builder_.add_usage_seconds(usage_seconds);
  builder_.add_machine_id(machine_id);
  builder_.add_activity_type(activity_type);
  return builder_.Finish();
}

inline flatbuffers::Offset<Activity> CreateActivityDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    double time = 0.0,
    const char *machine_id = nullptr,
    ActivityType activity_type = ActivityType::Signed_In,
    uint32_t usage_seconds = 0,
    const char *tag_id = nullptr) {
  return ACM::CreateActivity(
      _fbb,
      time,
      machine_id ? _fbb.CreateString(machine_id) : 0,
      activity_type,
      usage_seconds,
      tag_id ? _fbb.CreateString(tag_id) : 0);
}

struct User FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return UserTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "ACM.User";
  }
  enum {
    VT_NAME = 4,
    VT_EMAIL = 6,
    VT_MAKERLABS_ID = 8,
    VT_MAKER_STATUS = 10,
    VT_TAG_ID = 12,
    VT_ALERTS = 14,
    VT_PERMISSIONS = 16
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  flatbuffers::String *mutable_name() {
    return GetPointer<flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *email() const {
    return GetPointer<const flatbuffers::String *>(VT_EMAIL);
  }
  flatbuffers::String *mutable_email() {
    return GetPointer<flatbuffers::String *>(VT_EMAIL);
  }
  const flatbuffers::String *makerlabs_id() const {
    return GetPointer<const flatbuffers::String *>(VT_MAKERLABS_ID);
  }
  flatbuffers::String *mutable_makerlabs_id() {
    return GetPointer<flatbuffers::String *>(VT_MAKERLABS_ID);
  }
  const flatbuffers::String *maker_status() const {
    return GetPointer<const flatbuffers::String *>(VT_MAKER_STATUS);
  }
  flatbuffers::String *mutable_maker_status() {
    return GetPointer<flatbuffers::String *>(VT_MAKER_STATUS);
  }
  const flatbuffers::String *tag_id() const {
    return GetPointer<const flatbuffers::String *>(VT_TAG_ID);
  }
  flatbuffers::String *mutable_tag_id() {
    return GetPointer<flatbuffers::String *>(VT_TAG_ID);
  }
  const flatbuffers::String *alerts() const {
    return GetPointer<const flatbuffers::String *>(VT_ALERTS);
  }
  flatbuffers::String *mutable_alerts() {
    return GetPointer<flatbuffers::String *>(VT_ALERTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *permissions() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_PERMISSIONS);
  }
  flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *mutable_permissions() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_PERMISSIONS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyOffset(verifier, VT_EMAIL) &&
           verifier.Verify(email()) &&
           VerifyOffset(verifier, VT_MAKERLABS_ID) &&
           verifier.Verify(makerlabs_id()) &&
           VerifyOffset(verifier, VT_MAKER_STATUS) &&
           verifier.Verify(maker_status()) &&
           VerifyOffset(verifier, VT_TAG_ID) &&
           verifier.Verify(tag_id()) &&
           VerifyOffset(verifier, VT_ALERTS) &&
           verifier.Verify(alerts()) &&
           VerifyOffset(verifier, VT_PERMISSIONS) &&
           verifier.Verify(permissions()) &&
           verifier.VerifyVectorOfStrings(permissions()) &&
           verifier.EndTable();
  }
};

struct UserBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(User::VT_NAME, name);
  }
  void add_email(flatbuffers::Offset<flatbuffers::String> email) {
    fbb_.AddOffset(User::VT_EMAIL, email);
  }
  void add_makerlabs_id(flatbuffers::Offset<flatbuffers::String> makerlabs_id) {
    fbb_.AddOffset(User::VT_MAKERLABS_ID, makerlabs_id);
  }
  void add_maker_status(flatbuffers::Offset<flatbuffers::String> maker_status) {
    fbb_.AddOffset(User::VT_MAKER_STATUS, maker_status);
  }
  void add_tag_id(flatbuffers::Offset<flatbuffers::String> tag_id) {
    fbb_.AddOffset(User::VT_TAG_ID, tag_id);
  }
  void add_alerts(flatbuffers::Offset<flatbuffers::String> alerts) {
    fbb_.AddOffset(User::VT_ALERTS, alerts);
  }
  void add_permissions(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> permissions) {
    fbb_.AddOffset(User::VT_PERMISSIONS, permissions);
  }
  explicit UserBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  UserBuilder &operator=(const UserBuilder &);
  flatbuffers::Offset<User> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<User>(end);
    return o;
  }
};

inline flatbuffers::Offset<User> CreateUser(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> email = 0,
    flatbuffers::Offset<flatbuffers::String> makerlabs_id = 0,
    flatbuffers::Offset<flatbuffers::String> maker_status = 0,
    flatbuffers::Offset<flatbuffers::String> tag_id = 0,
    flatbuffers::Offset<flatbuffers::String> alerts = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> permissions = 0) {
  UserBuilder builder_(_fbb);
  builder_.add_permissions(permissions);
  builder_.add_alerts(alerts);
  builder_.add_tag_id(tag_id);
  builder_.add_maker_status(maker_status);
  builder_.add_makerlabs_id(makerlabs_id);
  builder_.add_email(email);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<User> CreateUserDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *email = nullptr,
    const char *makerlabs_id = nullptr,
    const char *maker_status = nullptr,
    const char *tag_id = nullptr,
    const char *alerts = nullptr,
    const std::vector<flatbuffers::Offset<flatbuffers::String>> *permissions = nullptr) {
  return ACM::CreateUser(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      email ? _fbb.CreateString(email) : 0,
      makerlabs_id ? _fbb.CreateString(makerlabs_id) : 0,
      maker_status ? _fbb.CreateString(maker_status) : 0,
      tag_id ? _fbb.CreateString(tag_id) : 0,
      alerts ? _fbb.CreateString(alerts) : 0,
      permissions ? _fbb.CreateVector<flatbuffers::Offset<flatbuffers::String>>(*permissions) : 0);
}

struct Log FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  static const flatbuffers::TypeTable *MiniReflectTypeTable() {
    return LogTypeTable();
  }
  static FLATBUFFERS_CONSTEXPR const char *GetFullyQualifiedName() {
    return "ACM.Log";
  }
  enum {
    VT_MACHINE_ID = 4,
    VT_SEVERITY = 6,
    VT_TIME = 8,
    VT_MESSAGE = 10
  };
  const flatbuffers::String *machine_id() const {
    return GetPointer<const flatbuffers::String *>(VT_MACHINE_ID);
  }
  flatbuffers::String *mutable_machine_id() {
    return GetPointer<flatbuffers::String *>(VT_MACHINE_ID);
  }
  LogSeverity severity() const {
    return static_cast<LogSeverity>(GetField<int8_t>(VT_SEVERITY, 0));
  }
  bool mutate_severity(LogSeverity _severity) {
    return SetField<int8_t>(VT_SEVERITY, static_cast<int8_t>(_severity), 0);
  }
  uint64_t time() const {
    return GetField<uint64_t>(VT_TIME, 0);
  }
  bool mutate_time(uint64_t _time) {
    return SetField<uint64_t>(VT_TIME, _time, 0);
  }
  const flatbuffers::String *message() const {
    return GetPointer<const flatbuffers::String *>(VT_MESSAGE);
  }
  flatbuffers::String *mutable_message() {
    return GetPointer<flatbuffers::String *>(VT_MESSAGE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_MACHINE_ID) &&
           verifier.Verify(machine_id()) &&
           VerifyField<int8_t>(verifier, VT_SEVERITY) &&
           VerifyField<uint64_t>(verifier, VT_TIME) &&
           VerifyOffset(verifier, VT_MESSAGE) &&
           verifier.Verify(message()) &&
           verifier.EndTable();
  }
};

struct LogBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_machine_id(flatbuffers::Offset<flatbuffers::String> machine_id) {
    fbb_.AddOffset(Log::VT_MACHINE_ID, machine_id);
  }
  void add_severity(LogSeverity severity) {
    fbb_.AddElement<int8_t>(Log::VT_SEVERITY, static_cast<int8_t>(severity), 0);
  }
  void add_time(uint64_t time) {
    fbb_.AddElement<uint64_t>(Log::VT_TIME, time, 0);
  }
  void add_message(flatbuffers::Offset<flatbuffers::String> message) {
    fbb_.AddOffset(Log::VT_MESSAGE, message);
  }
  explicit LogBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  LogBuilder &operator=(const LogBuilder &);
  flatbuffers::Offset<Log> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Log>(end);
    return o;
  }
};

inline flatbuffers::Offset<Log> CreateLog(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> machine_id = 0,
    LogSeverity severity = LogSeverity::Fatal,
    uint64_t time = 0,
    flatbuffers::Offset<flatbuffers::String> message = 0) {
  LogBuilder builder_(_fbb);
  builder_.add_time(time);
  builder_.add_message(message);
  builder_.add_machine_id(machine_id);
  builder_.add_severity(severity);
  return builder_.Finish();
}

inline flatbuffers::Offset<Log> CreateLogDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *machine_id = nullptr,
    LogSeverity severity = LogSeverity::Fatal,
    uint64_t time = 0,
    const char *message = nullptr) {
  return ACM::CreateLog(
      _fbb,
      machine_id ? _fbb.CreateString(machine_id) : 0,
      severity,
      time,
      message ? _fbb.CreateString(message) : 0);
}

inline const flatbuffers::TypeTable *ActivityTypeTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_CHAR, 0, 0 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    ActivityTypeTypeTable
  };
  static const char * const names[] = {
    "Signed_In",
    "Signed_Out",
    "CNC_Job"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_ENUM, 3, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *LogSeverityTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_CHAR, 0, 0 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    LogSeverityTypeTable
  };
  static const char * const names[] = {
    "Fatal",
    "Error",
    "Warn",
    "Notice",
    "Info",
    "Debug",
    "Trace"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_ENUM, 7, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *ActivityTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_DOUBLE, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_UINT, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    ActivityTypeTypeTable
  };
  static const char * const names[] = {
    "time",
    "machine_id",
    "activity_type",
    "usage_seconds",
    "tag_id"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 5, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *UserTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_STRING, 1, -1 }
  };
  static const char * const names[] = {
    "name",
    "email",
    "makerlabs_id",
    "maker_status",
    "tag_id",
    "alerts",
    "permissions"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 7, type_codes, nullptr, nullptr, names
  };
  return &tt;
}

inline const flatbuffers::TypeTable *LogTypeTable() {
  static const flatbuffers::TypeCode type_codes[] = {
    { flatbuffers::ET_STRING, 0, -1 },
    { flatbuffers::ET_CHAR, 0, 0 },
    { flatbuffers::ET_ULONG, 0, -1 },
    { flatbuffers::ET_STRING, 0, -1 }
  };
  static const flatbuffers::TypeFunction type_refs[] = {
    LogSeverityTypeTable
  };
  static const char * const names[] = {
    "machine_id",
    "severity",
    "time",
    "message"
  };
  static const flatbuffers::TypeTable tt = {
    flatbuffers::ST_TABLE, 4, type_codes, type_refs, nullptr, names
  };
  return &tt;
}

inline const ACM::Activity *GetActivity(const void *buf) {
  return flatbuffers::GetRoot<ACM::Activity>(buf);
}

inline const ACM::Activity *GetSizePrefixedActivity(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<ACM::Activity>(buf);
}

inline Activity *GetMutableActivity(void *buf) {
  return flatbuffers::GetMutableRoot<Activity>(buf);
}

inline const char *ActivityIdentifier() {
  return "ACM.";
}

inline bool ActivityBufferHasIdentifier(const void *buf) {
  return flatbuffers::BufferHasIdentifier(
      buf, ActivityIdentifier());
}

inline bool VerifyActivityBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<ACM::Activity>(ActivityIdentifier());
}

inline bool VerifySizePrefixedActivityBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<ACM::Activity>(ActivityIdentifier());
}

inline const char *ActivityExtension() {
  return "fb";
}

inline void FinishActivityBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<ACM::Activity> root) {
  fbb.Finish(root, ActivityIdentifier());
}

inline void FinishSizePrefixedActivityBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<ACM::Activity> root) {
  fbb.FinishSizePrefixed(root, ActivityIdentifier());
}

}  // namespace ACM

#endif  // FLATBUFFERS_GENERATED_ACM_ACM_H_
