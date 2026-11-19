# Modules and API

[Usage](usage.md) · [README](../README.md) · written by `tools/surface.py` from `tools/surface.json` (audited by `tools/check_exports.py`).

Every exported name is the upstream entity, in the namespace upstream declares it in. No `absl` name is exported:
protobuf's API uses Abseil types (`absl::Status`, `absl::string_view`, `absl::Cord`); to name them, import
`mcpplibs.mwrap.abseil` or include the Abseil header.

| Module | Names | Of |
|---|---:|---|
| `mcpplibs.mwrap.protobuf` | 710 | messages, descriptors, reflection, arenas, repeated fields, maps, text format and the well-known types |
| `mcpplibs.mwrap.protobuf.io` | 33 | coded and zero-copy streams, the tokenizer and the printer |
| `mcpplibs.mwrap.protobuf.util` | 51 | JSON, MessageDifferencer, TimeUtil, FieldMaskUtil, delimited messages and type resolvers |
| `mcpplibs.mwrap.protobuf.compiler` | 7 | parsing .proto files at run time: Importer, DiskSourceTree, Parser |
| `mcpplibs.mwrap.protobuf.io.gzip` | 2 | GzipInputStream and GzipOutputStream (the package's `gzip` feature) |
| `mcpplibs.mwrap.protobuf.macros` | 7 | the version macros and the tag computation |

## mcpplibs.mwrap.protobuf

- `google::protobuf` types (227): `AccessListener`, `Any`, `AnyGlobalsTypeInternal`, `Api`, `ApiGlobalsTypeInternal`, `Arena`, `ArenaOptions`, `AssignDescriptorsHelper`, `BigEndian`, `BoolValue`, `BoolValueGlobalsTypeInternal`, `BytesValue`, `BytesValueGlobalsTypeInternal`, `Closure`, `ConstMapIterator`, `DebugStringOptions`, `Descriptor`, `DescriptorBuilder`, `DescriptorDatabase`, `DescriptorPool`, `DescriptorPoolDatabase`, `DescriptorPoolDatabaseOptions`, `DescriptorProto`, `DescriptorProtoGlobalsTypeInternal`, `DescriptorProto_ExtensionRange`, `DescriptorProto_ExtensionRangeGlobalsTypeInternal`, `DescriptorProto_ReservedRange`, `DescriptorProto_ReservedRangeGlobalsTypeInternal`, `DoubleValue`, `DoubleValueGlobalsTypeInternal`, `Duration`, `DurationGlobalsTypeInternal`, `DynamicMapSorter`, `DynamicMessage`, `DynamicMessageFactory`, `Edition`, `Empty`, `EmptyGlobalsTypeInternal`, `EncodedDescriptorDatabase`, `Enum`, `EnumDescriptor`, `EnumDescriptorProto`, `EnumDescriptorProtoGlobalsTypeInternal`, `EnumDescriptorProto_EnumReservedRange`, `EnumDescriptorProto_EnumReservedRangeGlobalsTypeInternal`, `EnumGlobalsTypeInternal`, `EnumOptions`, `EnumOptionsGlobalsTypeInternal`, `EnumValue`, `EnumValueDescriptor`, `EnumValueDescriptorProto`, `EnumValueDescriptorProtoGlobalsTypeInternal`, `EnumValueGlobalsTypeInternal`, `EnumValueOptions`, `EnumValueOptionsGlobalsTypeInternal`, `ExtDeclEnforcementLevel`, `ExtensionRangeOptions`, `ExtensionRangeOptionsGlobalsTypeInternal`, `ExtensionRangeOptions_Declaration`, `ExtensionRangeOptions_DeclarationGlobalsTypeInternal`, `ExtensionRangeOptions_VerificationState`, `FastReflectionMessageMutator`, `FastReflectionStringSetter`, `FeatureResolver`, `FeatureSet`, `FeatureSetDefaults`, `FeatureSetDefaultsGlobalsTypeInternal`, `FeatureSetDefaults_FeatureSetEditionDefault`, `FeatureSetDefaults_FeatureSetEditionDefaultGlobalsTypeInternal`, `FeatureSetGlobalsTypeInternal`, `FeatureSet_EnforceNamingStyle`, `FeatureSet_EnumType`, `FeatureSet_FieldPresence`, `FeatureSet_JsonFormat`, `FeatureSet_MessageEncoding`, `FeatureSet_RepeatedFieldEncoding`, `FeatureSet_Utf8Validation`, `FeatureSet_VisibilityFeature`, `FeatureSet_VisibilityFeatureGlobalsTypeInternal`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility`, `Field`, `FieldDescriptor`, `FieldDescriptorProto`, `FieldDescriptorProtoGlobalsTypeInternal`, `FieldDescriptorProto_Label`, `FieldDescriptorProto_Type`, `FieldGlobalsTypeInternal`, `FieldMask`, `FieldMaskGlobalsTypeInternal`, `FieldOptions`, `FieldOptionsGlobalsTypeInternal`, `FieldOptions_CType`, `FieldOptions_EditionDefault`, `FieldOptions_EditionDefaultGlobalsTypeInternal`, `FieldOptions_FeatureSupport`, `FieldOptions_FeatureSupportGlobalsTypeInternal`, `FieldOptions_JSType`, `FieldOptions_OptionRetention`, `FieldOptions_OptionTargetType`, `Field_Cardinality`, `Field_Kind`, `FileDescriptor`, `FileDescriptorLegacy`, `FileDescriptorProto`, `FileDescriptorProtoGlobalsTypeInternal`, `FileDescriptorSet`, `FileDescriptorSetGlobalsTypeInternal`, `FileDescriptorTables`, `FileOptions`, `FileOptionsGlobalsTypeInternal`, `FileOptions_OptimizeMode`, `FloatValue`, `FloatValueGlobalsTypeInternal`, `GeneratedCodeInfo`, `GeneratedCodeInfoGlobalsTypeInternal`, `GeneratedCodeInfo_Annotation`, `GeneratedCodeInfo_AnnotationGlobalsTypeInternal`, `GeneratedCodeInfo_Annotation_Semantic`, `GeneratedMessageReflectionTestHelper`, `Int32Value`, `Int32ValueGlobalsTypeInternal`, `Int64Value`, `Int64ValueGlobalsTypeInternal`, `ListValue`, `ListValueGlobalsTypeInternal`, `MakeDependent`, `Map`, `MapIterator`, `MapIteratorBase`, `MapKey`, `MapPair`, `MapReflectionTester`, `MapValueConstRef`, `MapValueRef`, `MergedDescriptorDatabase`, `Message`, `MessageFactory`, `MessageLayoutInspector`, `MessageLite`, `MessageOptions`, `MessageOptionsGlobalsTypeInternal`, `Metadata`, `Method`, `MethodDescriptor`, `MethodDescriptorProto`, `MethodDescriptorProtoGlobalsTypeInternal`, `MethodGlobalsTypeInternal`, `MethodOptions`, `MethodOptionsGlobalsTypeInternal`, `MethodOptions_IdempotencyLevel`, `Mixin`, `MixinGlobalsTypeInternal`, `MutableRepeatedFieldRef`, `NoOpAccessListener`, `NullValue`, `OneofDescriptor`, `OneofDescriptorProto`, `OneofDescriptorProtoGlobalsTypeInternal`, `OneofOptions`, `OneofOptionsGlobalsTypeInternal`, `Option`, `OptionGlobalsTypeInternal`, `Reflection`, `RepeatedField`, `RepeatedFieldProxy`, `RepeatedFieldRef`, `RepeatedPtrField`, `ResultCallback`, `ResultCallback1`, `ResultCallback2`, `RpcChannel`, `RpcController`, `Service`, `ServiceDescriptor`, `ServiceDescriptorProto`, `ServiceDescriptorProtoGlobalsTypeInternal`, `ServiceOptions`, `ServiceOptionsGlobalsTypeInternal`, `SimpleDescriptorDatabase`, `SourceCodeInfo`, `SourceCodeInfoGlobalsTypeInternal`, `SourceCodeInfo_Location`, `SourceCodeInfo_LocationGlobalsTypeInternal`, `SourceContext`, `SourceContextGlobalsTypeInternal`, `SourceLocation`, `StringValue`, `StringValueGlobalsTypeInternal`, `Struct`, `StructGlobalsTypeInternal`, `Struct_FieldsEntry_DoNotUse`, `Struct_FieldsEntry_DoNotUseGlobalsTypeInternal`, `Symbol`, `SymbolChecker`, `SymbolCheckerError`, `SymbolCheckerErrorType`, `SymbolVisibility`, `Syntax`, `TextFormat`, `Timestamp`, `TimestampGlobalsTypeInternal`, `Type`, `TypeGlobalsTypeInternal`, `TypeId`, `UInt32Value`, `UInt32ValueGlobalsTypeInternal`, `UInt64Value`, `UInt64ValueGlobalsTypeInternal`, `UninterpretedOption`, `UninterpretedOptionGlobalsTypeInternal`, `UninterpretedOption_NamePart`, `UninterpretedOption_NamePartGlobalsTypeInternal`, `UnknownField`, `UnknownFieldSet`, `Value`, `ValueGlobalsTypeInternal`, `WeakRepeatedPtrField`, `int16`, `int32`, `int64`, `int8`, `is_proto_enum`, `uint`, `uint16`, `uint32`, `uint64`, `uint8`
- `google::protobuf` enumerators of unscoped enumerations (133): `EDITION_1_TEST_ONLY`, `EDITION_2023`, `EDITION_2024`, `EDITION_2026`, `EDITION_2_TEST_ONLY`, `EDITION_99997_TEST_ONLY`, `EDITION_99998_TEST_ONLY`, `EDITION_99999_TEST_ONLY`, `EDITION_LEGACY`, `EDITION_MAX`, `EDITION_PROTO2`, `EDITION_PROTO3`, `EDITION_UNKNOWN`, `EDITION_UNSTABLE`, `ExtensionRangeOptions_VerificationState_DECLARATION`, `ExtensionRangeOptions_VerificationState_UNVERIFIED`, `FeatureSet_EnforceNamingStyle_ENFORCE_NAMING_STYLE_UNKNOWN`, `FeatureSet_EnforceNamingStyle_STYLE2024`, `FeatureSet_EnforceNamingStyle_STYLE2026`, `FeatureSet_EnforceNamingStyle_STYLE_LEGACY`, `FeatureSet_EnumType_CLOSED`, `FeatureSet_EnumType_ENUM_TYPE_UNKNOWN`, `FeatureSet_EnumType_OPEN`, `FeatureSet_FieldPresence_EXPLICIT`, `FeatureSet_FieldPresence_FIELD_PRESENCE_UNKNOWN`, `FeatureSet_FieldPresence_IMPLICIT`, `FeatureSet_FieldPresence_LEGACY_REQUIRED`, `FeatureSet_JsonFormat_ALLOW`, `FeatureSet_JsonFormat_JSON_FORMAT_UNKNOWN`, `FeatureSet_JsonFormat_LEGACY_BEST_EFFORT`, `FeatureSet_MessageEncoding_DELIMITED`, `FeatureSet_MessageEncoding_LENGTH_PREFIXED`, `FeatureSet_MessageEncoding_MESSAGE_ENCODING_UNKNOWN`, `FeatureSet_RepeatedFieldEncoding_EXPANDED`, `FeatureSet_RepeatedFieldEncoding_PACKED`, `FeatureSet_RepeatedFieldEncoding_REPEATED_FIELD_ENCODING_UNKNOWN`, `FeatureSet_Utf8Validation_NONE`, `FeatureSet_Utf8Validation_UTF8_VALIDATION_UNKNOWN`, `FeatureSet_Utf8Validation_VERIFY`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_DEFAULT_SYMBOL_VISIBILITY_UNKNOWN`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_EXPORT_ALL`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_EXPORT_TOP_LEVEL`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_LOCAL_ALL`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_STRICT`, `FieldDescriptorProto_Label_LABEL_OPTIONAL`, `FieldDescriptorProto_Label_LABEL_REPEATED`, `FieldDescriptorProto_Label_LABEL_REQUIRED`, `FieldDescriptorProto_Type_TYPE_BOOL`, `FieldDescriptorProto_Type_TYPE_BYTES`, `FieldDescriptorProto_Type_TYPE_DOUBLE`, `FieldDescriptorProto_Type_TYPE_ENUM`, `FieldDescriptorProto_Type_TYPE_FIXED32`, `FieldDescriptorProto_Type_TYPE_FIXED64`, `FieldDescriptorProto_Type_TYPE_FLOAT`, `FieldDescriptorProto_Type_TYPE_GROUP`, `FieldDescriptorProto_Type_TYPE_INT32`, `FieldDescriptorProto_Type_TYPE_INT64`, `FieldDescriptorProto_Type_TYPE_MESSAGE`, `FieldDescriptorProto_Type_TYPE_SFIXED32`, `FieldDescriptorProto_Type_TYPE_SFIXED64`, `FieldDescriptorProto_Type_TYPE_SINT32`, `FieldDescriptorProto_Type_TYPE_SINT64`, `FieldDescriptorProto_Type_TYPE_STRING`, `FieldDescriptorProto_Type_TYPE_UINT32`, `FieldDescriptorProto_Type_TYPE_UINT64`, `FieldOptions_CType_CORD`, `FieldOptions_CType_STRING`, `FieldOptions_CType_STRING_PIECE`, `FieldOptions_JSType_JS_NORMAL`, `FieldOptions_JSType_JS_NUMBER`, `FieldOptions_JSType_JS_STRING`, `FieldOptions_OptionRetention_RETENTION_RUNTIME`, `FieldOptions_OptionRetention_RETENTION_SOURCE`, `FieldOptions_OptionRetention_RETENTION_UNKNOWN`, `FieldOptions_OptionTargetType_TARGET_TYPE_ENUM`, `FieldOptions_OptionTargetType_TARGET_TYPE_ENUM_ENTRY`, `FieldOptions_OptionTargetType_TARGET_TYPE_EXTENSION_RANGE`, `FieldOptions_OptionTargetType_TARGET_TYPE_FIELD`, `FieldOptions_OptionTargetType_TARGET_TYPE_FILE`, `FieldOptions_OptionTargetType_TARGET_TYPE_MESSAGE`, `FieldOptions_OptionTargetType_TARGET_TYPE_METHOD`, `FieldOptions_OptionTargetType_TARGET_TYPE_ONEOF`, `FieldOptions_OptionTargetType_TARGET_TYPE_SERVICE`, `FieldOptions_OptionTargetType_TARGET_TYPE_UNKNOWN`, `Field_Cardinality_CARDINALITY_OPTIONAL`, `Field_Cardinality_CARDINALITY_REPEATED`, `Field_Cardinality_CARDINALITY_REQUIRED`, `Field_Cardinality_CARDINALITY_UNKNOWN`, `Field_Cardinality_Field_Cardinality_INT_MAX_SENTINEL_DO_NOT_USE_`, `Field_Cardinality_Field_Cardinality_INT_MIN_SENTINEL_DO_NOT_USE_`, `Field_Kind_Field_Kind_INT_MAX_SENTINEL_DO_NOT_USE_`, `Field_Kind_Field_Kind_INT_MIN_SENTINEL_DO_NOT_USE_`, `Field_Kind_TYPE_BOOL`, `Field_Kind_TYPE_BYTES`, `Field_Kind_TYPE_DOUBLE`, `Field_Kind_TYPE_ENUM`, `Field_Kind_TYPE_FIXED32`, `Field_Kind_TYPE_FIXED64`, `Field_Kind_TYPE_FLOAT`, `Field_Kind_TYPE_GROUP`, `Field_Kind_TYPE_INT32`, `Field_Kind_TYPE_INT64`, `Field_Kind_TYPE_MESSAGE`, `Field_Kind_TYPE_SFIXED32`, `Field_Kind_TYPE_SFIXED64`, `Field_Kind_TYPE_SINT32`, `Field_Kind_TYPE_SINT64`, `Field_Kind_TYPE_STRING`, `Field_Kind_TYPE_UINT32`, `Field_Kind_TYPE_UINT64`, `Field_Kind_TYPE_UNKNOWN`, `FileOptions_OptimizeMode_CODE_SIZE`, `FileOptions_OptimizeMode_LITE_RUNTIME`, `FileOptions_OptimizeMode_SPEED`, `GeneratedCodeInfo_Annotation_Semantic_ALIAS`, `GeneratedCodeInfo_Annotation_Semantic_NONE`, `GeneratedCodeInfo_Annotation_Semantic_SET`, `MethodOptions_IdempotencyLevel_IDEMPOTENCY_UNKNOWN`, `MethodOptions_IdempotencyLevel_IDEMPOTENT`, `MethodOptions_IdempotencyLevel_NO_SIDE_EFFECTS`, `NESTED_ENUM_STRICT_VIOLATION`, `NESTED_MESSAGE_STRICT_VIOLATION`, `NULL_VALUE`, `NullValue_INT_MAX_SENTINEL_DO_NOT_USE_`, `NullValue_INT_MIN_SENTINEL_DO_NOT_USE_`, `SYNTAX_EDITIONS`, `SYNTAX_PROTO2`, `SYNTAX_PROTO3`, `Syntax_INT_MAX_SENTINEL_DO_NOT_USE_`, `Syntax_INT_MIN_SENTINEL_DO_NOT_USE_`, `VISIBILITY_EXPORT`, `VISIBILITY_LOCAL`, `VISIBILITY_UNSET`
- `google::protobuf` objects (217): `Any_class_data_`, `Any_globals_`, `Api_class_data_`, `Api_globals_`, `BoolValue_class_data_`, `BoolValue_globals_`, `BytesValue_class_data_`, `BytesValue_globals_`, `DescriptorProto_ExtensionRange_class_data_`, `DescriptorProto_ExtensionRange_globals_`, `DescriptorProto_ReservedRange_class_data_`, `DescriptorProto_ReservedRange_globals_`, `DescriptorProto_class_data_`, `DescriptorProto_globals_`, `DoubleValue_class_data_`, `DoubleValue_globals_`, `Duration_class_data_`, `Duration_globals_`, `Edition_MAX`, `Edition_MIN`, `Edition_internal_data_`, `Empty_class_data_`, `Empty_globals_`, `EnumDescriptorProto_EnumReservedRange_class_data_`, `EnumDescriptorProto_EnumReservedRange_globals_`, `EnumDescriptorProto_class_data_`, `EnumDescriptorProto_globals_`, `EnumOptions_class_data_`, `EnumOptions_globals_`, `EnumValueDescriptorProto_class_data_`, `EnumValueDescriptorProto_globals_`, `EnumValueOptions_class_data_`, `EnumValueOptions_globals_`, `EnumValue_class_data_`, `EnumValue_globals_`, `Enum_class_data_`, `Enum_globals_`, `ExtensionRangeOptions_Declaration_class_data_`, `ExtensionRangeOptions_Declaration_globals_`, `ExtensionRangeOptions_VerificationState_VerificationState_ARRAYSIZE`, `ExtensionRangeOptions_VerificationState_VerificationState_MAX`, `ExtensionRangeOptions_VerificationState_VerificationState_MIN`, `ExtensionRangeOptions_VerificationState_internal_data_`, `ExtensionRangeOptions_class_data_`, `ExtensionRangeOptions_globals_`, `FeatureSetDefaults_FeatureSetEditionDefault_class_data_`, `FeatureSetDefaults_FeatureSetEditionDefault_globals_`, `FeatureSetDefaults_class_data_`, `FeatureSetDefaults_globals_`, `FeatureSet_EnforceNamingStyle_EnforceNamingStyle_ARRAYSIZE`, `FeatureSet_EnforceNamingStyle_EnforceNamingStyle_MAX`, `FeatureSet_EnforceNamingStyle_EnforceNamingStyle_MIN`, `FeatureSet_EnforceNamingStyle_internal_data_`, `FeatureSet_EnumType_EnumType_ARRAYSIZE`, `FeatureSet_EnumType_EnumType_MAX`, `FeatureSet_EnumType_EnumType_MIN`, `FeatureSet_EnumType_internal_data_`, `FeatureSet_FieldPresence_FieldPresence_ARRAYSIZE`, `FeatureSet_FieldPresence_FieldPresence_MAX`, `FeatureSet_FieldPresence_FieldPresence_MIN`, `FeatureSet_FieldPresence_internal_data_`, `FeatureSet_JsonFormat_JsonFormat_ARRAYSIZE`, `FeatureSet_JsonFormat_JsonFormat_MAX`, `FeatureSet_JsonFormat_JsonFormat_MIN`, `FeatureSet_JsonFormat_internal_data_`, `FeatureSet_MessageEncoding_MessageEncoding_ARRAYSIZE`, `FeatureSet_MessageEncoding_MessageEncoding_MAX`, `FeatureSet_MessageEncoding_MessageEncoding_MIN`, `FeatureSet_MessageEncoding_internal_data_`, `FeatureSet_RepeatedFieldEncoding_RepeatedFieldEncoding_ARRAYSIZE`, `FeatureSet_RepeatedFieldEncoding_RepeatedFieldEncoding_MAX`, `FeatureSet_RepeatedFieldEncoding_RepeatedFieldEncoding_MIN`, `FeatureSet_RepeatedFieldEncoding_internal_data_`, `FeatureSet_Utf8Validation_Utf8Validation_ARRAYSIZE`, `FeatureSet_Utf8Validation_Utf8Validation_MAX`, `FeatureSet_Utf8Validation_Utf8Validation_MIN`, `FeatureSet_Utf8Validation_internal_data_`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_DefaultSymbolVisibility_ARRAYSIZE`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_DefaultSymbolVisibility_MAX`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_DefaultSymbolVisibility_MIN`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_internal_data_`, `FeatureSet_VisibilityFeature_class_data_`, `FeatureSet_VisibilityFeature_globals_`, `FeatureSet_class_data_`, `FeatureSet_globals_`, `FieldDescriptorProto_Label_Label_ARRAYSIZE`, `FieldDescriptorProto_Label_Label_MAX`, `FieldDescriptorProto_Label_Label_MIN`, `FieldDescriptorProto_Label_internal_data_`, `FieldDescriptorProto_Type_Type_ARRAYSIZE`, `FieldDescriptorProto_Type_Type_MAX`, `FieldDescriptorProto_Type_Type_MIN`, `FieldDescriptorProto_Type_internal_data_`, `FieldDescriptorProto_class_data_`, `FieldDescriptorProto_globals_`, `FieldMask_class_data_`, `FieldMask_globals_`, `FieldOptions_CType_CType_ARRAYSIZE`, `FieldOptions_CType_CType_MAX`, `FieldOptions_CType_CType_MIN`, `FieldOptions_CType_internal_data_`, `FieldOptions_EditionDefault_class_data_`, `FieldOptions_EditionDefault_globals_`, `FieldOptions_FeatureSupport_class_data_`, `FieldOptions_FeatureSupport_globals_`, `FieldOptions_JSType_JSType_ARRAYSIZE`, `FieldOptions_JSType_JSType_MAX`, `FieldOptions_JSType_JSType_MIN`, `FieldOptions_JSType_internal_data_`, `FieldOptions_OptionRetention_OptionRetention_ARRAYSIZE`, `FieldOptions_OptionRetention_OptionRetention_MAX`, `FieldOptions_OptionRetention_OptionRetention_MIN`, `FieldOptions_OptionRetention_internal_data_`, `FieldOptions_OptionTargetType_OptionTargetType_ARRAYSIZE`, `FieldOptions_OptionTargetType_OptionTargetType_MAX`, `FieldOptions_OptionTargetType_OptionTargetType_MIN`, `FieldOptions_OptionTargetType_internal_data_`, `FieldOptions_class_data_`, `FieldOptions_globals_`, `Field_Cardinality_Cardinality_ARRAYSIZE`, `Field_Cardinality_Cardinality_MAX`, `Field_Cardinality_Cardinality_MIN`, `Field_Cardinality_internal_data_`, `Field_Kind_Kind_ARRAYSIZE`, `Field_Kind_Kind_MAX`, `Field_Kind_Kind_MIN`, `Field_Kind_internal_data_`, `Field_class_data_`, `Field_globals_`, `FileDescriptorProto_class_data_`, `FileDescriptorProto_globals_`, `FileDescriptorSet_class_data_`, `FileDescriptorSet_globals_`, `FileOptions_OptimizeMode_OptimizeMode_ARRAYSIZE`, `FileOptions_OptimizeMode_OptimizeMode_MAX`, `FileOptions_OptimizeMode_OptimizeMode_MIN`, `FileOptions_OptimizeMode_internal_data_`, `FileOptions_class_data_`, `FileOptions_globals_`, `FloatValue_class_data_`, `FloatValue_globals_`, `GeneratedCodeInfo_Annotation_Semantic_Semantic_ARRAYSIZE`, `GeneratedCodeInfo_Annotation_Semantic_Semantic_MAX`, `GeneratedCodeInfo_Annotation_Semantic_Semantic_MIN`, `GeneratedCodeInfo_Annotation_Semantic_internal_data_`, `GeneratedCodeInfo_Annotation_class_data_`, `GeneratedCodeInfo_Annotation_globals_`, `GeneratedCodeInfo_class_data_`, `GeneratedCodeInfo_globals_`, `Int32Value_class_data_`, `Int32Value_globals_`, `Int64Value_class_data_`, `Int64Value_globals_`, `ListValue_class_data_`, `ListValue_globals_`, `MessageOptions_class_data_`, `MessageOptions_globals_`, `MethodDescriptorProto_class_data_`, `MethodDescriptorProto_globals_`, `MethodOptions_IdempotencyLevel_IdempotencyLevel_ARRAYSIZE`, `MethodOptions_IdempotencyLevel_IdempotencyLevel_MAX`, `MethodOptions_IdempotencyLevel_IdempotencyLevel_MIN`, `MethodOptions_IdempotencyLevel_internal_data_`, `MethodOptions_class_data_`, `MethodOptions_globals_`, `Method_class_data_`, `Method_globals_`, `Mixin_class_data_`, `Mixin_globals_`, `NullValue_ARRAYSIZE`, `NullValue_MAX`, `NullValue_MIN`, `NullValue_internal_data_`, `OneofDescriptorProto_class_data_`, `OneofDescriptorProto_globals_`, `OneofOptions_class_data_`, `OneofOptions_globals_`, `Option_class_data_`, `Option_globals_`, `ServiceDescriptorProto_class_data_`, `ServiceDescriptorProto_globals_`, `ServiceOptions_class_data_`, `ServiceOptions_globals_`, `SourceCodeInfo_Location_class_data_`, `SourceCodeInfo_Location_globals_`, `SourceCodeInfo_class_data_`, `SourceCodeInfo_globals_`, `SourceContext_class_data_`, `SourceContext_globals_`, `StringValue_class_data_`, `StringValue_globals_`, `Struct_FieldsEntry_DoNotUse_class_data_`, `Struct_FieldsEntry_DoNotUse_globals_`, `Struct_class_data_`, `Struct_globals_`, `SymbolVisibility_ARRAYSIZE`, `SymbolVisibility_MAX`, `SymbolVisibility_MIN`, `SymbolVisibility_internal_data_`, `Syntax_ARRAYSIZE`, `Syntax_MAX`, `Syntax_MIN`, `Syntax_internal_data_`, `Timestamp_class_data_`, `Timestamp_globals_`, `Type_class_data_`, `Type_globals_`, `UInt32Value_class_data_`, `UInt32Value_globals_`, `UInt64Value_class_data_`, `UInt64Value_globals_`, `UninterpretedOption_NamePart_class_data_`, `UninterpretedOption_NamePart_globals_`, `UninterpretedOption_class_data_`, `UninterpretedOption_globals_`, `Value_class_data_`, `Value_globals_`
- `google::protobuf` functions and operators (130): `AbslStringify`, `AllocatedRepeatedPtrFieldBackInserter`, `DoNothing`, `DownCastMessage`, `DownCastToGenerated`, `DynamicCastMessage`, `DynamicCastToGenerated`, `Edition_IsValid`, `Edition_Name`, `Edition_Parse`, `Edition_descriptor`, `EnumerateEnumValues`, `ExtensionRangeOptions_VerificationState_IsValid`, `ExtensionRangeOptions_VerificationState_Name`, `ExtensionRangeOptions_VerificationState_Parse`, `ExtensionRangeOptions_VerificationState_descriptor`, `FeatureSet_EnforceNamingStyle_IsValid`, `FeatureSet_EnforceNamingStyle_Name`, `FeatureSet_EnforceNamingStyle_Parse`, `FeatureSet_EnforceNamingStyle_descriptor`, `FeatureSet_EnumType_IsValid`, `FeatureSet_EnumType_Name`, `FeatureSet_EnumType_Parse`, `FeatureSet_EnumType_descriptor`, `FeatureSet_FieldPresence_IsValid`, `FeatureSet_FieldPresence_Name`, `FeatureSet_FieldPresence_Parse`, `FeatureSet_FieldPresence_descriptor`, `FeatureSet_JsonFormat_IsValid`, `FeatureSet_JsonFormat_Name`, `FeatureSet_JsonFormat_Parse`, `FeatureSet_JsonFormat_descriptor`, `FeatureSet_MessageEncoding_IsValid`, `FeatureSet_MessageEncoding_Name`, `FeatureSet_MessageEncoding_Parse`, `FeatureSet_MessageEncoding_descriptor`, `FeatureSet_RepeatedFieldEncoding_IsValid`, `FeatureSet_RepeatedFieldEncoding_Name`, `FeatureSet_RepeatedFieldEncoding_Parse`, `FeatureSet_RepeatedFieldEncoding_descriptor`, `FeatureSet_Utf8Validation_IsValid`, `FeatureSet_Utf8Validation_Name`, `FeatureSet_Utf8Validation_Parse`, `FeatureSet_Utf8Validation_descriptor`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_IsValid`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_Name`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_Parse`, `FeatureSet_VisibilityFeature_DefaultSymbolVisibility_descriptor`, `FieldDescriptorProto_Label_IsValid`, `FieldDescriptorProto_Label_Name`, `FieldDescriptorProto_Label_Parse`, `FieldDescriptorProto_Label_descriptor`, `FieldDescriptorProto_Type_IsValid`, `FieldDescriptorProto_Type_Name`, `FieldDescriptorProto_Type_Parse`, `FieldDescriptorProto_Type_descriptor`, `FieldOptions_CType_IsValid`, `FieldOptions_CType_Name`, `FieldOptions_CType_Parse`, `FieldOptions_CType_descriptor`, `FieldOptions_JSType_IsValid`, `FieldOptions_JSType_Name`, `FieldOptions_JSType_Parse`, `FieldOptions_JSType_descriptor`, `FieldOptions_OptionRetention_IsValid`, `FieldOptions_OptionRetention_Name`, `FieldOptions_OptionRetention_Parse`, `FieldOptions_OptionRetention_descriptor`, `FieldOptions_OptionTargetType_IsValid`, `FieldOptions_OptionTargetType_Name`, `FieldOptions_OptionTargetType_Parse`, `FieldOptions_OptionTargetType_descriptor`, `Field_Cardinality_IsValid`, `Field_Cardinality_Name`, `Field_Cardinality_Parse`, `Field_Cardinality_descriptor`, `Field_Kind_IsValid`, `Field_Kind_Name`, `Field_Kind_Parse`, `Field_Kind_descriptor`, `FileOptions_OptimizeMode_IsValid`, `FileOptions_OptimizeMode_Name`, `FileOptions_OptimizeMode_Parse`, `FileOptions_OptimizeMode_descriptor`, `GOOGLE_UNALIGNED_LOAD16`, `GOOGLE_UNALIGNED_LOAD32`, `GOOGLE_UNALIGNED_LOAD64`, `GOOGLE_UNALIGNED_STORE16`, `GOOGLE_UNALIGNED_STORE32`, `GOOGLE_UNALIGNED_STORE64`, `GeneratedCodeInfo_Annotation_Semantic_IsValid`, `GeneratedCodeInfo_Annotation_Semantic_Name`, `GeneratedCodeInfo_Annotation_Semantic_Parse`, `GeneratedCodeInfo_Annotation_Semantic_descriptor`, `GetEnumDescriptor`, `GetExtensionReflection`, `LinkExtensionReflection`, `LinkMessageReflection`, `MethodOptions_IdempotencyLevel_IsValid`, `MethodOptions_IdempotencyLevel_Name`, `MethodOptions_IdempotencyLevel_Parse`, `MethodOptions_IdempotencyLevel_descriptor`, `NewCallback`, `NewPermanentCallback`, `NullValue_IsValid`, `NullValue_Name`, `NullValue_Parse`, `NullValue_descriptor`, `ProtobufInternalGetEnumDescriptor`, `RepeatedFieldBackInserter`, `RepeatedPtrFieldBackInserter`, `ShortFormat`, `ShutdownProtobufLibrary`, `SymbolVisibility_IsValid`, `SymbolVisibility_Name`, `SymbolVisibility_Parse`, `SymbolVisibility_descriptor`, `Syntax_IsValid`, `Syntax_Name`, `Syntax_Parse`, `Syntax_descriptor`, `UnsafeArenaAllocatedRepeatedPtrFieldBackInserter`, `Utf8Format`, `c_sort`, `c_stable_sort`, `erase`, `erase_if`, `ghtonl`, `sort`, `stable_sort`
- `google::protobuf` names protobuf re-declares from another namespace (2): `AbslParseFlag`, `AbslUnparseFlag`
- `google::protobuf::internal` operators of public types declared in an implementation namespace, for argument-dependent lookup (1): `operator|`

## mcpplibs.mwrap.protobuf.io

- `google::protobuf` types (1): `ZeroCopyCodedInputStream`
- `google::protobuf::io` types (26): `AnnotationCollector`, `AnnotationProtoCollector`, `ArrayInputStream`, `ArrayOutputStream`, `CodedInputStream`, `CodedOutputStream`, `ColumnNumber`, `ConcatenatingInputStream`, `CopyingInputStream`, `CopyingInputStreamAdaptor`, `CopyingOutputStream`, `CopyingOutputStreamAdaptor`, `CordInputStream`, `CordOutputStream`, `EpsCopyOutputStream`, `ErrorCollector`, `FileInputStream`, `FileOutputStream`, `IstreamInputStream`, `LimitingInputStream`, `OstreamOutputStream`, `Printer`, `StringOutputStream`, `Tokenizer`, `ZeroCopyInputStream`, `ZeroCopyOutputStream`
- `google::protobuf::io` functions and operators (6): `NoLocaleStrtod`, `SafeDoubleToFloat`, `SimpleDtoa`, `SimpleFtoa`, `as_string_data`, `mutable_string_data`

## mcpplibs.mwrap.protobuf.util

- `google::protobuf` functions and operators (17): `operator!=`, `operator%`, `operator%=`, `operator*`, `operator*=`, `operator+`, `operator+=`, `operator-`, `operator-=`, `operator/`, `operator/=`, `operator<`, `operator<<`, `operator<=`, `operator==`, `operator>`, `operator>=`
- `google::protobuf::json` types (2): `ParseOptions`, `PrintOptions`
- `google::protobuf::json` functions and operators (8): `BinaryToJsonStream`, `BinaryToJsonString`, `JsonStreamToMessage`, `JsonStringToMessage`, `JsonToBinaryStream`, `JsonToBinaryString`, `MessageToJsonStream`, `MessageToJsonString`
- `google::protobuf::util` types (10): `DefaultFieldComparator`, `FieldComparator`, `FieldContext`, `FieldMaskUtil`, `JsonParseOptions`, `JsonPrintOptions`, `MessageDifferencer`, `SimpleFieldComparator`, `TimeUtil`, `TypeResolver`
- `google::protobuf::util` functions and operators (8): `ConvertDescriptorToType`, `NewTypeResolverForDescriptorPool`, `ParseDelimitedFromCodedStream`, `ParseDelimitedFromZeroCopyStream`, `SerializeDelimitedToCodedStream`, `SerializeDelimitedToFileDescriptor`, `SerializeDelimitedToOstream`, `SerializeDelimitedToZeroCopyStream`
- `google::protobuf::util` names protobuf re-declares from another namespace (6): `BinaryToJsonStream`, `BinaryToJsonString`, `JsonStringToMessage`, `JsonToBinaryStream`, `JsonToBinaryString`, `MessageToJsonString`

## mcpplibs.mwrap.protobuf.compiler

- `google::protobuf::compiler` types (7): `DiskSourceTree`, `Importer`, `MultiFileErrorCollector`, `Parser`, `SourceLocationTable`, `SourceTree`, `SourceTreeDescriptorDatabase`

## mcpplibs.mwrap.protobuf.io.gzip

- `google::protobuf::io` types (2): `GzipInputStream`, `GzipOutputStream`

## Not exported

- Headers:
  - `google/protobuf/arena_test_util.h`: test support
  - `google/protobuf/map_test_util.h`: test support (needs test protos)
  - `google/protobuf/reflection_tester.h`: test support (needs test protos)
  - `google/protobuf/test_util.h`: test support
  - `google/protobuf/test_util2.h`: test support
  - `google/protobuf/test_util_lite.h`: test support
  - `google/protobuf/test_textproto.h`: test support (needs GoogleMock)
  - `google/protobuf/wire_format_unittest.h`: test support
  - `google/protobuf/descriptor_test_utils.h`: test support
  - `google/protobuf/internal_visibility_for_testing.h`: test support
  - `google/protobuf/unredacted_debug_format_for_test.h`: test support
  - `google/protobuf/io/test_zero_copy_stream.h`: test support
  - `google/protobuf/io/io_win32.h`: Windows-only portability layer of the compiler (namespace io::win32)
  - `google/protobuf/util/internal_timeval.h`: implementation detail
- Implementation namespaces: `google::protobuf::compiler::generator_internal`, `google::protobuf::internal`, `google::protobuf::internal::big_endian`, `google::protobuf::internal::cleanup`, `google::protobuf::internal::cpp`, `google::protobuf::internal::detail`, `google::protobuf::internal::field_layout`, `google::protobuf::internal::generated_enum`, `google::protobuf::internal::little_endian`, `google::protobuf::internal::string_util`, `google::protobuf::io::zc_sink_internal`
- Namespace `pb`: the C++ feature-set extension of editions (pb::CppFeatures, from cpp_features.proto; extension_set.h forward-declares it), used by protoc and generated code: not exported, so a module-only translation unit keeps the common `namespace pb = google::protobuf;`; <google/protobuf/cpp_features.pb.h> declares it
- Names:
  - `google::protobuf::kint32max`: internal linkage: declared again with its value in mcpplibs::mwrap::protobuf
  - `google::protobuf::kint32min`: internal linkage: declared again with its value in mcpplibs::mwrap::protobuf
  - `google::protobuf::kint64max`: internal linkage: declared again with its value in mcpplibs::mwrap::protobuf
  - `google::protobuf::kint64min`: internal linkage: declared again with its value in mcpplibs::mwrap::protobuf
  - `google::protobuf::kuint32max`: internal linkage: declared again with its value in mcpplibs::mwrap::protobuf
  - `google::protobuf::kuint64max`: internal linkage: declared again with its value in mcpplibs::mwrap::protobuf
  - `google::protobuf::TestUtil::ReflectionTester`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::compiler::CodeGenerator`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::compiler::CommandLineInterface`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::compiler::cpp::CalculateFieldDescriptorRepeatedType`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::compiler::cpp::CppGenerator`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::compiler::cpp::Formatter`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::compiler::cpp::MessageTableTester`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::compiler::java::MemoizeProjection`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::descriptor_unittest::DescriptorPoolMemoizationTest`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::descriptor_unittest::DescriptorTest`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::descriptor_unittest::FeaturesTest`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::descriptor_unittest::ValidationErrorTest`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::expr::CelMapReflectionFriend`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::expr::SudoMapReflectionFriend`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::python::MapReflectionFriend`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::python::MessageReflectionFriend`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::python::cmessage::PythonFieldValuePrinter`: only declared (a friend of a protobuf class): defined by another project or by protoc
  - `google::protobuf::rust::PtrAndLen`: only declared (a friend of a protobuf class): defined by another project or by protoc

## Macros

- Constants in `mcpplibs::mwrap::protobuf::macros`: `GOOGLE_PROTOBUF_VERSION`, `GOOGLE_PROTOBUF_VERSION_SUFFIX`, `PROTOBUF_OSS_VERSION`, `PROTOBUF_OSS_VERSION_SUFFIX`, `PROTOBUF_VERSION`, `PROTOBUF_VERSION_SUFFIX`
- Functions: `GOOGLE_PROTOBUF_WIRE_FORMAT_MAKE_TAG`
- Through `<mwrap/protobuf/macros.hpp>` only:
  - `GOOGLE_PROTOBUF_ANY_H__`: header guard
  - `GOOGLE_PROTOBUF_ARCH_64_BIT`: describes the target (GOOGLE_PROTOBUF_ARCH_*): a switch for #if, different on each platform
  - `GOOGLE_PROTOBUF_ARCH_X64`: describes the target (GOOGLE_PROTOBUF_ARCH_*): a switch for #if, different on each platform
  - `GOOGLE_PROTOBUF_ARENASTRING_H__`: header guard
  - `GOOGLE_PROTOBUF_ARENAZ_SAMPLER_H__`: header guard
  - `GOOGLE_PROTOBUF_ARENA_ALIGN_H__`: header guard
  - `GOOGLE_PROTOBUF_ARENA_ALLOCATION_POLICY_H__`: header guard
  - `GOOGLE_PROTOBUF_ARENA_CLEANUP_H__`: header guard
  - `GOOGLE_PROTOBUF_ARENA_H__`: header guard
  - `GOOGLE_PROTOBUF_ASSERT_RETURN`: function-like: generates code or attributes, or returns from its caller
  - `GOOGLE_PROTOBUF_COMMON_H__`: header guard
  - `GOOGLE_PROTOBUF_COMPILER_IMPORTER_H__`: header guard
  - `GOOGLE_PROTOBUF_COMPILER_PARSER_H__`: header guard
  - `GOOGLE_PROTOBUF_CPP_EDITION_DEFAULTS_H__`: header guard
  - `GOOGLE_PROTOBUF_DESCRIPTOR_DATABASE_H__`: header guard
  - `GOOGLE_PROTOBUF_DESCRIPTOR_H__`: header guard
  - `GOOGLE_PROTOBUF_DESCRIPTOR_LEGACY_H__`: header guard
  - `GOOGLE_PROTOBUF_DESCRIPTOR_LITE_H__`: header guard
  - `GOOGLE_PROTOBUF_DESCRIPTOR_VISITOR_H__`: header guard
  - `GOOGLE_PROTOBUF_DYNAMIC_MESSAGE_H__`: header guard
  - `GOOGLE_PROTOBUF_ENDIAN_H__`: header guard
  - `GOOGLE_PROTOBUF_EXPLICITLY_CONSTRUCTED_H__`: header guard
  - `GOOGLE_PROTOBUF_EXTENSION_SET_H__`: header guard
  - `GOOGLE_PROTOBUF_EXTENSION_SET_INL_H__`: header guard
  - `GOOGLE_PROTOBUF_FEATURE_RESOLVER_H__`: header guard
  - `GOOGLE_PROTOBUF_FIELD_ACCESS_LISTENER_H__`: header guard
  - `GOOGLE_PROTOBUF_FIELD_WITH_ARENA_H__`: header guard
  - `GOOGLE_PROTOBUF_GENERATED_ENUM_REFLECTION_H__`: header guard
  - `GOOGLE_PROTOBUF_GENERATED_ENUM_UTIL_H__`: header guard
  - `GOOGLE_PROTOBUF_GENERATED_MESSAGE_BASES_H__`: header guard
  - `GOOGLE_PROTOBUF_GENERATED_MESSAGE_REFLECTION_H__`: header guard
  - `GOOGLE_PROTOBUF_GENERATED_MESSAGE_TCTABLE_DECL_H__`: header guard
  - `GOOGLE_PROTOBUF_GENERATED_MESSAGE_TCTABLE_GEN_H__`: header guard
  - `GOOGLE_PROTOBUF_GENERATED_MESSAGE_TCTABLE_IMPL_H__`: header guard
  - `GOOGLE_PROTOBUF_GENERATED_MESSAGE_UTIL_H__`: header guard
  - `GOOGLE_PROTOBUF_HAS_BITS_H__`: header guard
  - `GOOGLE_PROTOBUF_HAS_CEL_MAP_REFLECTION_FRIEND`: empty: an annotation or a switch for #ifdef
  - `GOOGLE_PROTOBUF_IMPLICIT_WEAK_MESSAGE_H__`: header guard
  - `GOOGLE_PROTOBUF_INLINED_STRING_FIELD_H__`: header guard
  - `GOOGLE_PROTOBUF_INTERNAL_FEATURE_HELPER_H__`: header guard
  - `GOOGLE_PROTOBUF_INTERNAL_METADATA_LOCATOR_H__`: header guard
  - `GOOGLE_PROTOBUF_INTERNAL_VISIBILITY_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_CODED_STREAM_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_GZIP_STREAM_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_PRINTER_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_STRTOD_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_TOKENIZER_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_ZERO_COPY_SINK_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_ZERO_COPY_STREAM_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_ZERO_COPY_STREAM_IMPL_H__`: header guard
  - `GOOGLE_PROTOBUF_IO_ZERO_COPY_STREAM_IMPL_LITE_H__`: header guard
  - `GOOGLE_PROTOBUF_JSON_JSON_H__`: header guard
  - `GOOGLE_PROTOBUF_MAP_ENTRY_H__`: header guard
  - `GOOGLE_PROTOBUF_MAP_FIELD_H__`: header guard
  - `GOOGLE_PROTOBUF_MAP_FIELD_LITE_H__`: header guard
  - `GOOGLE_PROTOBUF_MAP_H__`: header guard
  - `GOOGLE_PROTOBUF_MAP_TYPE_HANDLER_H__`: header guard
  - `GOOGLE_PROTOBUF_MESSAGE_H__`: header guard
  - `GOOGLE_PROTOBUF_MESSAGE_LITE_H__`: header guard
  - `GOOGLE_PROTOBUF_METADATA_H__`: header guard
  - `GOOGLE_PROTOBUF_METADATA_LITE_H__`: header guard
  - `GOOGLE_PROTOBUF_MICRO_STRING_H__`: header guard
  - `GOOGLE_PROTOBUF_PARSER_ASSERT`: function-like: generates code or attributes, or returns from its caller
  - `GOOGLE_PROTOBUF_PARSE_CONTEXT_H__`: header guard
  - `GOOGLE_PROTOBUF_PLATFORM_MACROS_H_`: header guard
  - `GOOGLE_PROTOBUF_PORT_H__`: header guard
  - `GOOGLE_PROTOBUF_RAW_PTR_H__`: header guard
  - `GOOGLE_PROTOBUF_REFLECTION_H__`: header guard
  - `GOOGLE_PROTOBUF_REFLECTION_INTERNAL_H__`: header guard
  - `GOOGLE_PROTOBUF_REFLECTION_MODE_H__`: header guard
  - `GOOGLE_PROTOBUF_REFLECTION_OPS_H__`: header guard
  - `GOOGLE_PROTOBUF_REFLECTION_VISIT_FIELDS_H__`: header guard
  - `GOOGLE_PROTOBUF_REFLECTION_VISIT_FIELD_INFO_H__`: header guard
  - `GOOGLE_PROTOBUF_REPEATED_FIELD_H__`: header guard
  - `GOOGLE_PROTOBUF_REPEATED_FIELD_PROXY_H__`: header guard
  - `GOOGLE_PROTOBUF_REPEATED_FIELD_PROXY_ITERATOR_H__`: header guard
  - `GOOGLE_PROTOBUF_REPEATED_FIELD_PROXY_TRAITS_H__`: header guard
  - `GOOGLE_PROTOBUF_REPEATED_PTR_FIELD_H__`: header guard
  - `GOOGLE_PROTOBUF_RUNTIME_DEPRECATED`: function-like: generates code or attributes, or returns from its caller
  - `GOOGLE_PROTOBUF_RUNTIME_VERSION_H__`: header guard
  - `GOOGLE_PROTOBUF_SERIAL_ARENA_H__`: header guard
  - `GOOGLE_PROTOBUF_SERVICE_H__`: header guard
  - `GOOGLE_PROTOBUF_STRING_BLOCK_H__`: header guard
  - `GOOGLE_PROTOBUF_STRING_MEMBER_ROBBER_H__`: header guard
  - `GOOGLE_PROTOBUF_STUBS_CALLBACK_H_`: header guard
  - `GOOGLE_PROTOBUF_STUBS_PORT_H_`: header guard
  - `GOOGLE_PROTOBUF_SYMBOL_CHECKER_H__`: header guard
  - `GOOGLE_PROTOBUF_TEXT_FORMAT_H__`: header guard
  - `GOOGLE_PROTOBUF_THREAD_SAFE_ARENA_H__`: header guard
  - `GOOGLE_PROTOBUF_UNKNOWN_FIELD_SET_H__`: header guard
  - `GOOGLE_PROTOBUF_UTIL_DELIMITED_MESSAGE_UTIL_H__`: header guard
  - `GOOGLE_PROTOBUF_UTIL_FIELD_COMPARATOR_H__`: header guard
  - `GOOGLE_PROTOBUF_UTIL_FIELD_MASK_UTIL_H__`: header guard
  - `GOOGLE_PROTOBUF_UTIL_JSON_UTIL_H__`: header guard
  - `GOOGLE_PROTOBUF_UTIL_MESSAGE_DIFFERENCER_H__`: header guard
  - `GOOGLE_PROTOBUF_UTIL_TIME_UTIL_H__`: header guard
  - `GOOGLE_PROTOBUF_UTIL_TYPE_RESOLVER_H__`: header guard
  - `GOOGLE_PROTOBUF_UTIL_TYPE_RESOLVER_UTIL_H__`: header guard
  - `GOOGLE_PROTOBUF_VARINT_SHUFFLE_H__`: header guard
  - `GOOGLE_PROTOBUF_VERIFY_VERSION`: a statement of generated code (calls the library's internals)
  - `GOOGLE_PROTOBUF_WIRE_FORMAT_H__`: header guard
  - `GOOGLE_PROTOBUF_WIRE_FORMAT_LITE_H__`: header guard
  - `LIBPROTOBUF_EXPORT`: empty: an annotation or a switch for #ifdef
  - `LIBPROTOC_EXPORT`: empty: an annotation or a switch for #ifdef
  - `PROTOBUF_DEFINE_NAME_ACCESSOR`: function-like: generates code or attributes, or returns from its caller
  - `PROTOBUF_DEFINE_OPTIONS_ACCESSOR`: function-like: generates code or attributes, or returns from its caller
  - `PROTOBUF_FUTURE_ADD_EARLY_NODISCARD`: annotation or an internal parameter list of generated code
  - `PROTOBUF_FUTURE_ADD_EARLY_WARN_UNUSED`: annotation or an internal parameter list of generated code
  - `PROTOBUF_FUTURE_ADD_NODISCARD`: empty: an annotation or a switch for #ifdef
  - `PROTOBUF_INTERNAL_CPP_EDITION_DEFAULTS`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_DIRECT_LAZY_FIELD_IN_EXTENSION_SET`: empty: an annotation or a switch for #ifdef
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fany_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fapi_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fcpp_5ffeatures_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fdescriptor_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fduration_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fempty_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2ffield_5fmask_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fsource_5fcontext_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fstruct_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2ftimestamp_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2ftype_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_INTERNAL_EXPORT_google_2fprotobuf_2fwrappers_2eproto`: annotation or an internal parameter list of generated code
  - `PROTOBUF_RUNTIME_DEPRECATED`: function-like: generates code or attributes, or returns from its caller
  - `PROTOBUF_TC_PARAM_NO_DATA_DECL`: annotation or an internal parameter list of generated code
  - `PROTOBUF_TC_PARAM_NO_DATA_PASS`: annotation or an internal parameter list of generated code
  - `PROTOBUF_TC_PARAM_PASS`: annotation or an internal parameter list of generated code
  - `PROTOBUF_TC_PARSE_FUNCTION_LIST`: annotation or an internal parameter list of generated code
  - `PROTOBUF_TC_PARSE_FUNCTION_LIST_END_GROUP`: function-like: generates code or attributes, or returns from its caller
  - `PROTOBUF_TC_PARSE_FUNCTION_LIST_PACKED`: function-like: generates code or attributes, or returns from its caller
  - `PROTOBUF_TC_PARSE_FUNCTION_LIST_REPEATED`: function-like: generates code or attributes, or returns from its caller
  - `PROTOBUF_TC_PARSE_FUNCTION_LIST_SINGLE`: function-like: generates code or attributes, or returns from its caller
  - `google_2fprotobuf_2fany_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2fapi_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2fcpp_5ffeatures_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2fdescriptor_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2fduration_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2fempty_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2ffield_5fmask_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2fsource_5fcontext_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2fstruct_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2ftimestamp_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2ftype_2eproto_2epb_2eh`: header guard
  - `google_2fprotobuf_2fwrappers_2eproto_2epb_2eh`: header guard
