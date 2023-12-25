/*
	"ESB" is designed to create external native components for the 1C(tm) program.
	Copyright � 2023 IntUnsigned	(v8classes@gmail.com)
	"ESB" is free software under LGPLv2.1 license with essential restriction about binary part (view esb-license.txt)
	"ESB" IS DISTRIBUTED "AS-IS" WITHOUT ANY, EVEN THE IMPLIED, WARRANTY. IN NO EVENT SHALL THE AUTHORS OR THE COPYRIGHT OWNER BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY ARISING OUT OF THE USE OF THIS SOFTWARE.
	THE AUTHOR DOES NOT GIVE ANY GUARANTEES AND DOES NOT BEAR ANY RESPONSIBILITY REGARDING THE USE OF 1C(tm) COMPONENTS PRODUCED USING THE ESB LIBRARY IN THE 1C(tm) ENVIRONMENT.
*/


#pragma once
#ifndef ESB_ES1_API_H_
#define ESB_ES1_API_H_


// ����� "�������" ���������, �, � ��� ����� ����������� ���������������� ������������ (ESB_USE_API_ALL, ESB_API_PROVIDER_INCLUDE)
#include "esb/esb_config_base.h"
#include "es1_api_providers.h"
#include "esdisp_api.h"
#include "es1_main.h"



namespace esb //API part
{
#if ESB_API_PROVIDER_INCLUDE(ext0xD)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ������� �������������(�����_ ��� �����, [���������������_ ��� ������], [���������������������������_ ��� ������]) ���������� ������
	inline String NumberInWords(ConstParam<Number> Number_, ConstParam<String> FormatString_ = DefUndef<String>, ConstParam<String> NumerationItemOptions_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, ext0xD, NumberInWords, Number_, FormatString_, NumerationItemOptions_); };
	// ������� ��������������������(�����������������_ ��� ����, ��������������������_ ��� ����, [���������������_ ��� ������]) ���������� ������
	inline String PeriodPresentation(ConstParam<DateTime> BeginningDateOfPeriod_, ConstParam<DateTime> EndingDateOfPeriod_, ConstParam<String> FormatString_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, ext0xD, PeriodPresentation, BeginningDateOfPeriod_, EndingDateOfPeriod_, FormatString_); };
	// ��������� ��������������(�����������������_ ��� ������, �����������������_ ��� ������)
	inline void FileCopy(ConstParam<String> SourceFileName_, ConstParam<String> ReceiverFileName_) { ESB_INVOKE_API_PROC2(ext0xD, FileCopy, SourceFileName_, ReceiverFileName_); };
	// ��������� ���������������(�����������������_ ��� ������, �����������������_ ��� ������)
	inline void MoveFile(ConstParam<String> SourceFileName_, ConstParam<String> ReceiverFileName_) { ESB_INVOKE_API_PROC2(ext0xD, MoveFile, SourceFileName_, ReceiverFileName_); };
	// ��������� ������������(����_ ��� ������, [�����_ ��� ������])
	inline void DeleteFiles(ConstParam<String> Path_, ConstParam<String> Mask_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(ext0xD, DeleteFiles, Path_, Mask_); };
	// ��������� ��������������(�����������_ ��� ������)
	inline void CreateDirectory(ConstParam<String> DirectoryName_) { ESB_INVOKE_API_PROC1(ext0xD, CreateDirectory, DirectoryName_); };
	// ��������� ���������������(������������_ ��� ������, �����������������������_ ��� ������)
	inline void MergeFiles(ConstParam<String> SearchTemplate_, ConstParam<String> ResultingFileName_) { ESB_INVOKE_API_PROC2(ext0xD, MergeFiles, SearchTemplate_, ResultingFileName_); };
	// ��������� ���������������(������������_ ��� ������, �����������������������_ ��� ������)
	inline void MergeFiles(ConstParam<Array> SegmentNames_, ConstParam<String> ResultingFileName_) { ESB_INVOKE_API_PROC2(ext0xD, MergeFiles, SegmentNames_, ResultingFileName_); };
	// ��������� �������������������(�������������_ ��� ������, [��������������_ ��� ������], [�������������������_ ��� ������], [�����������_ ��� ������<�����, ������������>])
	inline void RunApp(ConstParam<String> CommandString_, ConstParam<String> CurrentDirectory_ = DefUndef<String>, ConstParam<Boolean> WaitForCompletion_ = DefUndef<Boolean>, ConstParam<Number> ReturnCode_ = DefUndef<Number>) { ESB_INVOKE_API_PROC4(ext0xD, RunApp, CommandString_, CurrentDirectory_, WaitForCompletion_, ReturnCode_); };
	// ������� ��������������������������������������() ���������� �����
	inline Number CurrentUniversalDateInMilliseconds() { ESB_INVOKE_API_FUNC0(Number, ext0xD, CurrentUniversalDateInMilliseconds); };
	// ��������� ����������������������(������������������_ ��� ������������������, �����������������_ ��� ������, �����������������_ ��� ������)
	inline void BeginCopyingFile(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> SourceFileName_, ConstParam<String> ReceiverFileName_) { ESB_INVOKE_API_PROC3(ext0xD, BeginCopyingFile, NotifyDescription_, SourceFileName_, ReceiverFileName_); };
	// ��������� ����������������������(������������������_ ��� ������������������, �����������������_ ��� ������, �����������������_ ��� ������)
	inline void BeginMovingFile(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> SourceFileName_, ConstParam<String> ReceiverFileName_) { ESB_INVOKE_API_PROC3(ext0xD, BeginMovingFile, NotifyDescription_, SourceFileName_, ReceiverFileName_); };
	// ��������� ��������������������(������������������_ ��� ������������������, ����_ ��� ������, [�����_ ��� ������])
	inline void BeginDeletingFiles(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> Path_, ConstParam<String> Mask_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(ext0xD, BeginDeletingFiles, NotifyDescription_, Path_, Mask_); };
	// ��������� �����������������(������������������_ ��� ������������������, ����_ ��� ������, [�����_ ��� ������], [�������������������_ ��� ������])
	inline void BeginFindingFiles(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> Path_, ConstParam<String> Mask_ = DefUndef<String>, ConstParam<Boolean> SearchInSubdirectories_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC4(ext0xD, BeginFindingFiles, NotifyDescription_, Path_, Mask_, SearchInSubdirectories_); };
	// ��������� ����������������������(������������������_ ��� ������������������, �����������_ ��� ������)
	inline void BeginCreatingDirectory(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> DirectoryName_) { ESB_INVOKE_API_PROC2(ext0xD, BeginCreatingDirectory, NotifyDescription_, DirectoryName_); };
	// ��������� ����������������������([������������������_ ��� ������������������], �������������_ ��� ������, [��������������_ ��� ������], [�������������������_ ��� ������])
	inline void BeginRunningApplication(ConstParam<UndefOr<NotifyDescription>> NotifyDescription_, ConstParam<String> CommandLine_, ConstParam<String> CurrentDirectory_ = DefUndef<String>, ConstParam<Boolean> WaitForCompletion_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC4(ext0xD, BeginRunningApplication, NotifyDescription_, CommandLine_, CurrentDirectory_, WaitForCompletion_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function NumberInWords(Number Number_, String FormatString_ = <default>, String NumerationItemOptions_ = <default>) -> String
	inline ������ �������������(����������<�����> �����_, ����������<������> ���������������_ = ��������������������������<������>, ����������<������> ���������������������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC3(������, ext0xD, NumberInWords, �����_, ���������������_, ���������������������������_); };
	// Function PeriodPresentation(Date BeginningDateOfPeriod_, Date EndingDateOfPeriod_, String FormatString_ = <default>) -> String
	inline ������ ��������������������(����������<���������> �����������������_, ����������<���������> ��������������������_, ����������<������> ���������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC3(������, ext0xD, PeriodPresentation, �����������������_, ��������������������_, ���������������_); };
	// Void FileCopy(String SourceFileName_, String ReceiverFileName_)
	inline void ��������������(����������<������> �����������������_, ����������<������> �����������������_) { ESB_INVOKE_API_PROC2(ext0xD, FileCopy, �����������������_, �����������������_); };
	// Void MoveFile(String SourceFileName_, String ReceiverFileName_)
	inline void ���������������(����������<������> �����������������_, ����������<������> �����������������_) { ESB_INVOKE_API_PROC2(ext0xD, MoveFile, �����������������_, �����������������_); };
	// Void DeleteFiles(String Path_, String Mask_ = <default>)
	inline void ������������(����������<������> ����_, ����������<������> �����_ = ��������������������������<������>) { ESB_INVOKE_API_PROC2(ext0xD, DeleteFiles, ����_, �����_); };
	// Void CreateDirectory(String DirectoryName_)
	inline void ��������������(����������<������> �����������_) { ESB_INVOKE_API_PROC1(ext0xD, CreateDirectory, �����������_); };
	// Void MergeFiles(String SearchTemplate_, String ResultingFileName_)
	inline void ���������������(����������<������> ������������_, ����������<������> �����������������������_) { ESB_INVOKE_API_PROC2(ext0xD, MergeFiles, ������������_, �����������������������_); };
	// Void MergeFiles(String SearchTemplate_, String ResultingFileName_)
	inline void ���������������(����������<������> �����������_, ����������<������> �����������������������_) { ESB_INVOKE_API_PROC2(ext0xD, MergeFiles, �����������_, �����������������������_); };
	// Void RunApp(String CommandString_, String CurrentDirectory_ = <default>, Boolean WaitForCompletion_ = <default>, OneOf<Number, Undefined> ReturnCode_ = <default>)
	inline void �������������������(����������<������> �������������_, ����������<������> ��������������_ = ��������������������������<������>, ����������<������> �������������������_ = ��������������������������<������>, ����������<�����> �����������_ = ��������������������������<�����>) { ESB_INVOKE_API_PROC4(ext0xD, RunApp, �������������_, ��������������_, �������������������_, �����������_); };
	// Function CurrentUniversalDateInMilliseconds() -> Number
	inline ����� ��������������������������������������() { ESB_INVOKE_API_FUNC0(�����, ext0xD, CurrentUniversalDateInMilliseconds); };
	// Void BeginCopyingFile(NotifyDescription NotifyDescription_, String SourceFileName_, String ReceiverFileName_)
	inline void ����������������������(����������<������������������> ������������������_, ����������<������> �����������������_, ����������<������> �����������������_) { ESB_INVOKE_API_PROC3(ext0xD, BeginCopyingFile, ������������������_, �����������������_, �����������������_); };
	// Void BeginMovingFile(NotifyDescription NotifyDescription_, String SourceFileName_, String ReceiverFileName_)
	inline void ����������������������(����������<������������������> ������������������_, ����������<������> �����������������_, ����������<������> �����������������_) { ESB_INVOKE_API_PROC3(ext0xD, BeginMovingFile, ������������������_, �����������������_, �����������������_); };
	// Void BeginDeletingFiles(NotifyDescription NotifyDescription_, String Path_, String Mask_ = <default>)
	inline void ��������������������(����������<������������������> ������������������_, ����������<������> ����_, ����������<������> �����_ = ��������������������������<������>) { ESB_INVOKE_API_PROC3(ext0xD, BeginDeletingFiles, ������������������_, ����_, �����_); };
	// Void BeginFindingFiles(NotifyDescription NotifyDescription_, String Path_, String Mask_ = <default>, Boolean SearchInSubdirectories_ = <default>)
	inline void �����������������(����������<������������������> ������������������_, ����������<������> ����_, ����������<������> �����_ = ��������������������������<������>, ����������<������> �������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC4(ext0xD, BeginFindingFiles, ������������������_, ����_, �����_, �������������������_); };
	// Void BeginCreatingDirectory(NotifyDescription NotifyDescription_, String DirectoryName_)
	inline void ����������������������(����������<������������������> ������������������_, ����������<������> �����������_) { ESB_INVOKE_API_PROC2(ext0xD, BeginCreatingDirectory, ������������������_, �����������_); };
	// Void BeginRunningApplication(NotifyDescription NotifyDescription_ = <default>, String CommandLine_, String CurrentDirectory_ = <default>, Boolean WaitForCompletion_ = <default>)
	inline void ����������������������(����������<���������������<������������������>> ������������������_, ����������<������> �������������_, ����������<������> ��������������_ = ��������������������������<������>, ����������<������> �������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC4(ext0xD, BeginRunningApplication, ������������������_, �������������_, ��������������_, �������������������_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(ext0xD)
#if ESB_API_PROVIDER_INCLUDE(mngbase)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ������� ���������������������������(���_ ��� ������, [���������_ ��� ���������]) ���������� ������������
	inline Arbitrary GetFunctionalOption(ConstParam<String> Name_, ConstParam<Structure> Parameters_ = DefUndef<Structure>) { ESB_INVOKE_API_FUNC2(Arbitrary, mngbase, GetFunctionalOption, Name_, Parameters_); };
	// ������� �����������������������������(������_ ��� ������������, [�����_ ��� ������<������, �����������������������>]) ���������� ������
	inline String PutToTempStorage(ConstParam<Arbitrary> Data_, ConstParam<String> Address_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, mngbase, PutToTempStorage, Data_, Address_); };
	inline String PutToTempStorage(ConstParam<Arbitrary> Data_, ConstParam<UUIDValue> Address_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_FUNC2(String, mngbase, PutToTempStorage, Data_, Address_); };
	// ������� �����������������������������(�����_ ��� ������) ���������� ������������
	inline Arbitrary GetFromTempStorage(ConstParam<String> Address_) { ESB_INVOKE_API_FUNC1(Arbitrary, mngbase, GetFromTempStorage, Address_); };
	// ��������� ����������������������������(�����_ ��� ������)
	inline void DeleteFromTempStorage(ConstParam<String> Address_) { ESB_INVOKE_API_PROC1(mngbase, DeleteFromTempStorage, Address_); };
	// ������� ������������������������������������������(������������������������_ ��� ������������) ���������� ������
	inline String GetPredefinedValueFullName(ConstParam<Arbitrary> PredefinedValue_) { ESB_INVOKE_API_FUNC1(String, mngbase, GetPredefinedValueFullName, PredefinedValue_); };
	// ������� ����������������������������() ���������� ������
	inline String GetClientAllFilesMask() { ESB_INVOKE_API_FUNC0(String, mngbase, GetClientAllFilesMask); };
	// ������� ������������������������������() ���������� ������
	inline String GetClientPathSeparator() { ESB_INVOKE_API_FUNC0(String, mngbase, GetClientPathSeparator); };
	// ��������� ��������������������������������������������([��������������������������_ ��� ������])
	inline void ConnectExternalDataSource(ConstParam<String> ExternalDataSourceName_ = DefUndef<String>) { ESB_INVOKE_API_PROC1(mngbase, ConnectExternalDataSource, ExternalDataSourceName_); };
	// ��������� �������������������������������������������([��������������������������_ ��� ������])
	inline void DisconnectExternalDataSource(ConstParam<String> ExternalDataSourceName_ = DefUndef<String>) { ESB_INVOKE_API_PROC1(mngbase, DisconnectExternalDataSource, ExternalDataSourceName_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function GetFunctionalOption(String Name_, Structure Parameters_ = <default>) -> Arbitrary
	inline ������������ ���������������������������(����������<������> ���_, ����������<���������> ���������_ = ��������������������������<���������>) { ESB_INVOKE_API_FUNC2(������������, mngbase, GetFunctionalOption, ���_, ���������_); };
	// Function PutToTempStorage(Arbitrary Data_, OneOf<String, UUID> Address_ = <default>) -> String
	inline ������ �����������������������������(����������<������������> ������_, ����������<������> �����_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC2(������, mngbase, PutToTempStorage, ������_, �����_); };
	inline ������ �����������������������������(����������<������������> ������_, ����������<�����������������������> �����_ = ��������������������������<�����������������������>) { ESB_INVOKE_API_FUNC2(������, mngbase, PutToTempStorage, ������_, �����_); };
	// Function GetFromTempStorage(String Address_) -> Arbitrary
	inline ������������ �����������������������������(����������<������> �����_) { ESB_INVOKE_API_FUNC1(������������, mngbase, GetFromTempStorage, �����_); };
	// Void DeleteFromTempStorage(String Address_)
	inline void ����������������������������(����������<������> �����_) { ESB_INVOKE_API_PROC1(mngbase, DeleteFromTempStorage, �����_); };
	// Function GetPredefinedValueFullName(Arbitrary PredefinedValue_) -> String
	inline ������ ������������������������������������������(����������<������������> ������������������������_) { ESB_INVOKE_API_FUNC1(������, mngbase, GetPredefinedValueFullName, ������������������������_); };
	// Function GetClientAllFilesMask() -> String
	inline ������ ����������������������������() { ESB_INVOKE_API_FUNC0(������, mngbase, GetClientAllFilesMask); };
	// Function GetClientPathSeparator() -> String
	inline ������ ������������������������������() { ESB_INVOKE_API_FUNC0(������, mngbase, GetClientPathSeparator); };
	// Void ConnectExternalDataSource(String ExternalDataSourceName_ = <default>)
	inline void ��������������������������������������������(����������<������> ��������������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC1(mngbase, ConnectExternalDataSource, ��������������������������_); };
	// Void DisconnectExternalDataSource(String ExternalDataSourceName_ = <default>)
	inline void �������������������������������������������(����������<������> ��������������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC1(mngbase, DisconnectExternalDataSource, ��������������������������_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(mngbase)
#if ESB_API_PROVIDER_INCLUDE(ext0xA)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ��������� �����������������������������������(������������������_ ��� ������������������, ��������_ ��� ������)
	inline void BeginCreateBinaryDataFromFile(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> FileName_) { ESB_INVOKE_API_PROC2(ext0xA, BeginCreateBinaryDataFromFile, NotifyDescription_, FileName_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void BeginCreateBinaryDataFromFile(NotifyDescription NotifyDescription_, String FileName_)
	inline void �����������������������������������(����������<������������������> ������������������_, ����������<������> ��������_) { ESB_INVOKE_API_PROC2(ext0xA, BeginCreateBinaryDataFromFile, ������������������_, ��������_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(ext0xA)
#if ESB_API_PROVIDER_INCLUDE(frntend)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// �������� [[������,������]] ����������� ��� ����
	//-PROP: WorkingDate	 ! not writable in context, but writable in info !
	// �������� [[������������]] ��������������� ��� ������
	inline String GetLaunchParameter() { ESB_INVOKE_API_PROP_GET(String, frntend, LaunchParameter); };
	// ��������� ��������������(�������������������_ ��� ������<���������������������, ������>, [�������_ ��� �����], [���������_ ��� ������])
	inline void DoMessageBox(ConstParam<String> MessageText_, ConstParam<Number> Timeout_ = DefUndef<Number>, ConstParam<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, DoMessageBox, MessageText_, Timeout_, Title_); };
	inline void DoMessageBox(ConstParam<FormattedString> MessageText_, ConstParam<Number> Timeout_ = DefUndef<Number>, ConstParam<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, DoMessageBox, MessageText_, Timeout_, Title_); };
	// ������� ������������(������_ ��� ������, [���������_ ��� ������], [�����_ ��� �����], [���������������_ ��� ������]) ���������� ������
	inline Boolean InputString(ConstParam<String> String_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<Number> Length_ = DefUndef<Number>, ConstParam<Boolean> Multiline_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC4(Boolean, frntend, InputString, String_, Tooltip_, Length_, Multiline_); };
	// ������� �����������(�����_ ��� �����, [���������_ ��� ������], [�����_ ��� �����], [��������_ ��� �����]) ���������� ������
	inline Boolean InputNumber(ConstParam<Number> Number_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<Number> Length_ = DefUndef<Number>, ConstParam<Number> Precision_ = DefUndef<Number>) { ESB_INVOKE_API_FUNC4(Boolean, frntend, InputNumber, Number_, Tooltip_, Length_, Precision_); };
	// ������� ����������(����_ ��� ����, [���������_ ��� ������], [���������_ ��� ���������]) ���������� ������
	inline Boolean InputDate(ConstParam<DateTime> Date_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<DateFractions> DatePart_ = DefUndef<DateFractions>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, InputDate, Date_, Tooltip_, DatePart_); };
	// ������� ��������������(��������_ ��� ������������, [���������_ ��� ������], [���_ ��� ������<���, �������������>]) ���������� ������
	inline Boolean InputValue(ConstParam<Arbitrary> Value_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<TypeValue> type_ = DefUndef<TypeValue>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, InputValue, Value_, Tooltip_, type_); };
	inline Boolean InputValue(ConstParam<Arbitrary> Value_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<TypeDescription> type_ = DefUndef<TypeDescription>) { ESB_INVOKE_API_FUNC3(Boolean, frntend, InputValue, Value_, Tooltip_, type_); };
	// ��������� ���������������(��������_ ��� ������������)
	inline void OpenValue(ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_PROC1(frntend, OpenValue, Value_); };
	// ��������� ������()
	inline void Beep() { ESB_INVOKE_API_PROC0(frntend, Beep); };
	// ������� ������������������������() ���������� ������
	inline String GetCaption() { ESB_INVOKE_API_FUNC0(String, frntend, GetCaption); };
	// ��������� ��������������������������(���������_ ��� ������)
	inline void SetCaption(ConstParam<String> Title_) { ESB_INVOKE_API_PROC1(frntend, SetCaption, Title_); };
	// ��������� �����������������(���_ ��� ������, ��������_ ��� ������������)
	inline void SaveValue(ConstParam<String> Name_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_PROC2(frntend, SaveValue, Name_, Value_); };
	// ������� ��������������������(���_ ��� ������) ���������� ������������
	inline Arbitrary RestoreValue(ConstParam<String> Name_) { ESB_INVOKE_API_FUNC1(Arbitrary, frntend, RestoreValue, Name_); };
	// ��������� ������������������������������()
	inline void SaveUserSettings() { ESB_INVOKE_API_PROC0(frntend, SaveUserSettings); };
	// ��������� ����������������([��������������������������������������_ ��� ������], [�������������������_ ��� ������], [�����������_ ��� ������<�����, ������������>])
	inline void RunSystem(ConstParam<String> AdditionalParametersOfCommandLine_ = DefUndef<String>, ConstParam<Boolean> WaitForCompletion_ = DefUndef<Boolean>, ConstParam<Number> ReturnCode_ = DefUndef<Number>) { ESB_INVOKE_API_PROC3(frntend, RunSystem, AdditionalParametersOfCommandLine_, WaitForCompletion_, ReturnCode_); };
	// ��������� ����������������������([����������������������_ ��� ������], [��������������������������_ ��� ������], [��������������������������������������_ ��� ������])
	inline void Exit(ConstParam<Boolean> OptionRequest_ = DefUndef<Boolean>, ConstParam<Boolean> RestartOnCompletion_ = DefUndef<Boolean>, ConstParam<String> AdditionalParametersOfCommandLine_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(frntend, Exit, OptionRequest_, RestartOnCompletion_, AdditionalParametersOfCommandLine_); };
	// ��������� �����������������������([��������������������������_ ��� ������], [��������������������������������������_ ��� ������])
	inline void Terminate(ConstParam<Boolean> RestartOnCompletion_ = DefUndef<Boolean>, ConstParam<String> AdditionalParametersOfCommandLine_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(frntend, Terminate, RestartOnCompletion_, AdditionalParametersOfCommandLine_); };
	// ��������� ����������������������������(������������_ ��� ������, ��������_ ��� �����, [����������_ ��� ������])
	inline void AttachIdleHandler(ConstParam<String> ProcedureName_, ConstParam<Number> Interval_, ConstParam<Boolean> Single_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC3(frntend, AttachIdleHandler, ProcedureName_, Interval_, Single_); };
	// ��������� ���������������������������(������������_ ��� ������)
	inline void DetachIdleHandler(ConstParam<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, DetachIdleHandler, ProcedureName_); };
	// ��������� �������������������������������()
	inline void LockApplication() { ESB_INVOKE_API_PROC0(frntend, LockApplication); };
	// ��������� �����������������()
	inline void ClearMessages() { ESB_INVOKE_API_PROC0(frntend, ClearMessages); };
	// ��������� ���������([��������������_ ��� ������], [��������_ ��� �����], [���������_ ��� ������], [��������_ ��� ��������])
	inline void Status(ConstParam<String> MessageText_ = DefUndef<String>, ConstParam<Number> Progress_ = DefUndef<Number>, ConstParam<String> Explanation_ = DefUndef<String>, ConstParam<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_API_PROC4(frntend, Status, MessageText_, Progress_, Explanation_, Picture_); };
	// ��������� ����������([����������_ ��� ������], [��������_ ��� ������������], [��������_ ��� ������������])
	inline void Notify(ConstParam<String> EventName_ = DefUndef<String>, ConstParam<Arbitrary> Parameter_ = DefUndef<Arbitrary>, ConstParam<Arbitrary> Source_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC3(frntend, Notify, EventName_, Parameter_, Source_); };
	// ��������� �������������������������������()
	inline void UserInterruptProcessing() { ESB_INVOKE_API_PROC0(frntend, UserInterruptProcessing); };
	// ������� ������������������������() ���������� ������
	inline Boolean OpenHelpContent() { ESB_INVOKE_API_FUNC0(Boolean, frntend, OpenHelpContent); };
	// ������� ��������������������([��������������_ ��� ������]) ���������� ������
	inline Boolean OpenHelpIndex(ConstParam<String> SearchText_ = DefUndef<String>) { ESB_INVOKE_API_FUNC1(Boolean, frntend, OpenHelpIndex, SearchText_); };
	// ��������� ��������������()
	inline void CloseHelp() { ESB_INVOKE_API_PROC0(frntend, CloseHelp); };
	// ��������� ������������������������������(������������_ ��� ������)
	inline void AttachNotificationHandler(ConstParam<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, AttachNotificationHandler, ProcedureName_); };
	// ��������� �����������������������������(������������_ ��� ������)
	inline void DetachNotificationHandler(ConstParam<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, DetachNotificationHandler, ProcedureName_); };
	// ��������� ��������������������������(������������������_ ��� ������������������)
	inline void ShowErrorInfo(ConstParam<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_PROC1(frntend, ShowErrorInfo, ErrorInfo_); };
	// ��������� �������������������������([��������������_ ��� �����])
	inline void ProcessJobs(ConstParam<Number> ProcessingTime_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(frntend, ProcessJobs, ProcessingTime_); };
	// ������� ��������������������������������() ���������� ������
	inline String UserDataWorkDir() { ESB_INVOKE_API_FUNC0(String, frntend, UserDataWorkDir); };
	// ������� �����������������() ���������� ������
	inline String DocumentsDir() { ESB_INVOKE_API_FUNC0(String, frntend, DocumentsDir); };
	// ��������� ���������������������������������(������������������_ ��� ������������������)
	inline void BeginGettingDocumentsDir(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(frntend, BeginGettingDocumentsDir, NotifyDescription_); };
	// ��������� �������������������������������������������������(������������������_ ��� ������������������)
	inline void BeginGettingUserDataWorkDir(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(frntend, BeginGettingUserDataWorkDir, NotifyDescription_); };
	// ��������� ����������������������([�����������������������������_ ��� ������������������], �������������������_ ��� ������<���������������������, ������>, [�������_ ��� �����], [���������_ ��� ������])
	inline void ShowMessageBox(ConstParam<UndefOr<NotifyDescription>> NotifyDescriptionOnCompletion_, ConstParam<String> MessageText_, ConstParam<Number> Timeout_ = DefUndef<Number>, ConstParam<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(frntend, ShowMessageBox, NotifyDescriptionOnCompletion_, MessageText_, Timeout_, Title_); };
	inline void ShowMessageBox(ConstParam<UndefOr<NotifyDescription>> NotifyDescriptionOnCompletion_, ConstParam<FormattedString> MessageText_, ConstParam<Number> Timeout_ = DefUndef<Number>, ConstParam<String> Title_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(frntend, ShowMessageBox, NotifyDescriptionOnCompletion_, MessageText_, Timeout_, Title_); };
	// ��������� ����������������([�����������������������������_ ��� ������������������], ��������_ ��� ������������)
	inline void ShowValue(ConstParam<UndefOr<NotifyDescription>> NotifyDescriptionOnCompletion_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_PROC2(frntend, ShowValue, NotifyDescriptionOnCompletion_, Value_); };
	// ��������� ����������������(�����������������������������_ ��� ������������������, [����_ ��� ����], [���������_ ��� ������], [���������_ ��� ���������])
	inline void ShowInputDate(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<DateTime> Date_ = DefUndef<DateTime>, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<DateFractions> DatePart_ = DefUndef<DateFractions>) { ESB_INVOKE_API_PROC4(frntend, ShowInputDate, NotifyDescriptionOnCompletion_, Date_, Tooltip_, DatePart_); };
	// ��������� ��������������������(�����������������������������_ ��� ������������������, ��������_ ��� ������������, [���������_ ��� ������], [���_ ��� ������<���, �������������>])
	inline void ShowInputValue(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<Arbitrary> Value_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<TypeValue> type_ = DefUndef<TypeValue>) { ESB_INVOKE_API_PROC4(frntend, ShowInputValue, NotifyDescriptionOnCompletion_, Value_, Tooltip_, type_); };
	inline void ShowInputValue(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<Arbitrary> Value_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<TypeDescription> type_ = DefUndef<TypeDescription>) { ESB_INVOKE_API_PROC4(frntend, ShowInputValue, NotifyDescriptionOnCompletion_, Value_, Tooltip_, type_); };
	// ��������� ������������������(�����������������������������_ ��� ������������������, [������_ ��� ������], [���������_ ��� ������], [�����_ ��� �����], [���������������_ ��� ������])
	inline void ShowInputString(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<String> String_ = DefUndef<String>, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<Number> Length_ = DefUndef<Number>, ConstParam<Boolean> Multiline_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC5(frntend, ShowInputString, NotifyDescriptionOnCompletion_, String_, Tooltip_, Length_, Multiline_); };
	// ��������� �����������������(�����������������������������_ ��� ������������������, �����_ ��� �����, [���������_ ��� ������], [�����_ ��� �����], [��������_ ��� �����])
	inline void ShowInputNumber(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_, ConstParam<Number> Number_, ConstParam<String> Tooltip_ = DefUndef<String>, ConstParam<Number> Length_ = DefUndef<Number>, ConstParam<Number> Precision_ = DefUndef<Number>) { ESB_INVOKE_API_PROC5(frntend, ShowInputNumber, NotifyDescriptionOnCompletion_, Number_, Tooltip_, Length_, Precision_); };
	// ��������� ��������������������������������������������������������(������������_ ��� ������)
	inline void AttachLicensingClientParametersRequestHandler(ConstParam<String> ProcedureName_) { ESB_INVOKE_API_PROC1(frntend, AttachLicensingClientParametersRequestHandler, ProcedureName_); };
	// ��������� �������������������������������������������������������()
	inline void DetachLicensingClientParametersRequestHandler() { ESB_INVOKE_API_PROC0(frntend, DetachLicensingClientParametersRequestHandler); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Property [[Read,Write]] WorkingDate -> Date
	//-PROP: WorkingDate	 ! not writable in context, but writable in info !
	// Property [[ReadOnly]] LaunchParameter -> String
	inline ������ �����������������������() { ESB_INVOKE_API_PROP_GET(������, frntend, LaunchParameter); };
	// Void DoMessageBox(OneOf<FormattedString, String> MessageText_, Number Timeout_ = <default>, String Title_ = <default>)
	inline void ��������������(����������<������> �������������������_, ����������<�����> �������_ = ��������������������������<�����>, ����������<������> ���������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC3(frntend, DoMessageBox, �������������������_, �������_, ���������_); };
	inline void ��������������(����������<���������������������> �������������������_, ����������<�����> �������_ = ��������������������������<�����>, ����������<������> ���������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC3(frntend, DoMessageBox, �������������������_, �������_, ���������_); };
	// Function InputString(String String_, String Tooltip_ = <default>, Number Length_ = <default>, Boolean Multiline_ = <default>) -> Boolean
	inline ������ ������������(����������<������> ������_, ����������<������> ���������_ = ��������������������������<������>, ����������<�����> �����_ = ��������������������������<�����>, ����������<������> ���������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC4(������, frntend, InputString, ������_, ���������_, �����_, ���������������_); };
	// Function InputNumber(Number Number_, String Tooltip_ = <default>, Number Length_ = <default>, Number Precision_ = <default>) -> Boolean
	inline ������ �����������(����������<�����> �����_, ����������<������> ���������_ = ��������������������������<������>, ����������<�����> �����_ = ��������������������������<�����>, ����������<�����> ��������_ = ��������������������������<�����>) { ESB_INVOKE_API_FUNC4(������, frntend, InputNumber, �����_, ���������_, �����_, ��������_); };
	// Function InputDate(Date Date_, String Tooltip_ = <default>, DateFractions DatePart_ = <default>) -> Boolean
	inline ������ ����������(����������<���������> ����_, ����������<������> ���������_ = ��������������������������<������>, ����������<���������> ���������_ = ��������������������������<���������>) { ESB_INVOKE_API_FUNC3(������, frntend, InputDate, ����_, ���������_, ���������_); };
	// Function InputValue(Arbitrary Value_, String Tooltip_ = <default>, OneOf<Type, TypeDescription> Type_ = <default>) -> Boolean
	inline ������ ��������������(����������<������������> ��������_, ����������<������> ���������_ = ��������������������������<������>, ����������<���> ���_ = ��������������������������<���>) { ESB_INVOKE_API_FUNC3(������, frntend, InputValue, ��������_, ���������_, ���_); };
	inline ������ ��������������(����������<������������> ��������_, ����������<������> ���������_ = ��������������������������<������>, ����������<�������������> ���_ = ��������������������������<�������������>) { ESB_INVOKE_API_FUNC3(������, frntend, InputValue, ��������_, ���������_, ���_); };
	// Void OpenValue(Arbitrary Value_)
	inline void ���������������(����������<������������> ��������_) { ESB_INVOKE_API_PROC1(frntend, OpenValue, ��������_); };
	// Void Beep()
	inline void ������() { ESB_INVOKE_API_PROC0(frntend, Beep); };
	// Function GetCaption() -> String
	inline ������ ������������������������() { ESB_INVOKE_API_FUNC0(������, frntend, GetCaption); };
	// Void SetCaption(String Title_)
	inline void ��������������������������(����������<������> ���������_) { ESB_INVOKE_API_PROC1(frntend, SetCaption, ���������_); };
	// Void SaveValue(String Name_, Arbitrary Value_)
	inline void �����������������(����������<������> ���_, ����������<������������> ��������_) { ESB_INVOKE_API_PROC2(frntend, SaveValue, ���_, ��������_); };
	// Function RestoreValue(String Name_) -> Arbitrary
	inline ������������ ��������������������(����������<������> ���_) { ESB_INVOKE_API_FUNC1(������������, frntend, RestoreValue, ���_); };
	// Void SaveUserSettings()
	inline void ������������������������������() { ESB_INVOKE_API_PROC0(frntend, SaveUserSettings); };
	// Void RunSystem(String AdditionalParametersOfCommandLine_ = <default>, Boolean WaitForCompletion_ = <default>, OneOf<Number, Undefined> ReturnCode_ = <default>)
	inline void ����������������(����������<������> ��������������������������������������_ = ��������������������������<������>, ����������<������> �������������������_ = ��������������������������<������>, ����������<�����> �����������_ = ��������������������������<�����>) { ESB_INVOKE_API_PROC3(frntend, RunSystem, ��������������������������������������_, �������������������_, �����������_); };
	// Void Exit(Boolean OptionRequest_ = <default>, Boolean RestartOnCompletion_ = <default>, String AdditionalParametersOfCommandLine_ = <default>)
	inline void ����������������������(����������<������> ����������������������_ = ��������������������������<������>, ����������<������> ��������������������������_ = ��������������������������<������>, ����������<������> ��������������������������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC3(frntend, Exit, ����������������������_, ��������������������������_, ��������������������������������������_); };
	// Void Terminate(Boolean RestartOnCompletion_ = <default>, String AdditionalParametersOfCommandLine_ = <default>)
	inline void �����������������������(����������<������> ��������������������������_ = ��������������������������<������>, ����������<������> ��������������������������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC2(frntend, Terminate, ��������������������������_, ��������������������������������������_); };
	// Void AttachIdleHandler(String ProcedureName_, Number Interval_, Boolean Single_ = <default>)
	inline void ����������������������������(����������<������> ������������_, ����������<�����> ��������_, ����������<������> ����������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC3(frntend, AttachIdleHandler, ������������_, ��������_, ����������_); };
	// Void DetachIdleHandler(String ProcedureName_)
	inline void ���������������������������(����������<������> ������������_) { ESB_INVOKE_API_PROC1(frntend, DetachIdleHandler, ������������_); };
	// Void LockApplication()
	inline void �������������������������������() { ESB_INVOKE_API_PROC0(frntend, LockApplication); };
	// Void ClearMessages()
	inline void �����������������() { ESB_INVOKE_API_PROC0(frntend, ClearMessages); };
	// Void Status(String MessageText_ = <default>, Number Progress_ = <default>, String Explanation_ = <default>, Picture Picture_ = <default>)
	inline void ���������(����������<������> ��������������_ = ��������������������������<������>, ����������<�����> ��������_ = ��������������������������<�����>, ����������<������> ���������_ = ��������������������������<������>, ����������<��������> ��������_ = ��������������������������<��������>) { ESB_INVOKE_API_PROC4(frntend, Status, ��������������_, ��������_, ���������_, ��������_); };
	// Void Notify(String EventName_ = <default>, Arbitrary Parameter_ = <default>, Arbitrary Source_ = <default>)
	inline void ����������(����������<������> ����������_ = ��������������������������<������>, ����������<������������> ��������_ = ��������������������������<������������>, ����������<������������> ��������_ = ��������������������������<������������>) { ESB_INVOKE_API_PROC3(frntend, Notify, ����������_, ��������_, ��������_); };
	// Void UserInterruptProcessing()
	inline void �������������������������������() { ESB_INVOKE_API_PROC0(frntend, UserInterruptProcessing); };
	// Function OpenHelpContent() -> Boolean
	inline ������ ������������������������() { ESB_INVOKE_API_FUNC0(������, frntend, OpenHelpContent); };
	// Function OpenHelpIndex(String SearchText_ = <default>) -> Boolean
	inline ������ ��������������������(����������<������> ��������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC1(������, frntend, OpenHelpIndex, ��������������_); };
	// Void CloseHelp()
	inline void ��������������() { ESB_INVOKE_API_PROC0(frntend, CloseHelp); };
	// Void AttachNotificationHandler(String ProcedureName_)
	inline void ������������������������������(����������<������> ������������_) { ESB_INVOKE_API_PROC1(frntend, AttachNotificationHandler, ������������_); };
	// Void DetachNotificationHandler(String ProcedureName_)
	inline void �����������������������������(����������<������> ������������_) { ESB_INVOKE_API_PROC1(frntend, DetachNotificationHandler, ������������_); };
	// Void ShowErrorInfo(ErrorInfo ErrorInfo_)
	inline void ��������������������������(����������<������������������> ������������������_) { ESB_INVOKE_API_PROC1(frntend, ShowErrorInfo, ������������������_); };
	// Void ProcessJobs(Number ProcessingTime_ = <default>)
	inline void �������������������������(����������<�����> ��������������_ = ��������������������������<�����>) { ESB_INVOKE_API_PROC1(frntend, ProcessJobs, ��������������_); };
	// Function UserDataWorkDir() -> String
	inline ������ ��������������������������������() { ESB_INVOKE_API_FUNC0(������, frntend, UserDataWorkDir); };
	// Function DocumentsDir() -> String
	inline ������ �����������������() { ESB_INVOKE_API_FUNC0(������, frntend, DocumentsDir); };
	// Void BeginGettingDocumentsDir(NotifyDescription NotifyDescription_)
	inline void ���������������������������������(����������<������������������> ������������������_) { ESB_INVOKE_API_PROC1(frntend, BeginGettingDocumentsDir, ������������������_); };
	// Void BeginGettingUserDataWorkDir(NotifyDescription NotifyDescription_)
	inline void �������������������������������������������������(����������<������������������> ������������������_) { ESB_INVOKE_API_PROC1(frntend, BeginGettingUserDataWorkDir, ������������������_); };
	// Void ShowMessageBox(NotifyDescription NotifyDescriptionOnCompletion_ = <default>, OneOf<FormattedString, String> MessageText_, Number Timeout_ = <default>, String Title_ = <default>)
	inline void ����������������������(����������<���������������<������������������>> �����������������������������_, ����������<������> �������������������_, ����������<�����> �������_ = ��������������������������<�����>, ����������<������> ���������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC4(frntend, ShowMessageBox, �����������������������������_, �������������������_, �������_, ���������_); };
	inline void ����������������������(����������<���������������<������������������>> �����������������������������_, ����������<���������������������> �������������������_, ����������<�����> �������_ = ��������������������������<�����>, ����������<������> ���������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC4(frntend, ShowMessageBox, �����������������������������_, �������������������_, �������_, ���������_); };
	// Void ShowValue(NotifyDescription NotifyDescriptionOnCompletion_ = <default>, Arbitrary Value_)
	inline void ����������������(����������<���������������<������������������>> �����������������������������_, ����������<������������> ��������_) { ESB_INVOKE_API_PROC2(frntend, ShowValue, �����������������������������_, ��������_); };
	// Void ShowInputDate(NotifyDescription NotifyDescriptionOnCompletion_, Date Date_ = <default>, String Tooltip_ = <default>, DateFractions DatePart_ = <default>)
	inline void ����������������(����������<������������������> �����������������������������_, ����������<���������> ����_ = ��������������������������<���������>, ����������<������> ���������_ = ��������������������������<������>, ����������<���������> ���������_ = ��������������������������<���������>) { ESB_INVOKE_API_PROC4(frntend, ShowInputDate, �����������������������������_, ����_, ���������_, ���������_); };
	// Void ShowInputValue(NotifyDescription NotifyDescriptionOnCompletion_, Arbitrary Value_, String Tooltip_ = <default>, OneOf<Type, TypeDescription> Type_ = <default>)
	inline void ��������������������(����������<������������������> �����������������������������_, ����������<������������> ��������_, ����������<������> ���������_ = ��������������������������<������>, ����������<���> ���_ = ��������������������������<���>) { ESB_INVOKE_API_PROC4(frntend, ShowInputValue, �����������������������������_, ��������_, ���������_, ���_); };
	inline void ��������������������(����������<������������������> �����������������������������_, ����������<������������> ��������_, ����������<������> ���������_ = ��������������������������<������>, ����������<�������������> ���_ = ��������������������������<�������������>) { ESB_INVOKE_API_PROC4(frntend, ShowInputValue, �����������������������������_, ��������_, ���������_, ���_); };
	// Void ShowInputString(NotifyDescription NotifyDescriptionOnCompletion_, String String_ = <default>, String Tooltip_ = <default>, Number Length_ = <default>, Boolean Multiline_ = <default>)
	inline void ������������������(����������<������������������> �����������������������������_, ����������<������> ������_ = ��������������������������<������>, ����������<������> ���������_ = ��������������������������<������>, ����������<�����> �����_ = ��������������������������<�����>, ����������<������> ���������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC5(frntend, ShowInputString, �����������������������������_, ������_, ���������_, �����_, ���������������_); };
	// Void ShowInputNumber(NotifyDescription NotifyDescriptionOnCompletion_, Number Number_, String Tooltip_ = <default>, Number Length_ = <default>, Number Precision_ = <default>)
	inline void �����������������(����������<������������������> �����������������������������_, ����������<�����> �����_, ����������<������> ���������_ = ��������������������������<������>, ����������<�����> �����_ = ��������������������������<�����>, ����������<�����> ��������_ = ��������������������������<�����>) { ESB_INVOKE_API_PROC5(frntend, ShowInputNumber, �����������������������������_, �����_, ���������_, �����_, ��������_); };
	// Void AttachLicensingClientParametersRequestHandler(String ProcedureName_)
	inline void ��������������������������������������������������������(����������<������> ������������_) { ESB_INVOKE_API_PROC1(frntend, AttachLicensingClientParametersRequestHandler, ������������_); };
	// Void DetachLicensingClientParametersRequestHandler()
	inline void �������������������������������������������������������() { ESB_INVOKE_API_PROC0(frntend, DetachLicensingClientParametersRequestHandler); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(frntend)
#if ESB_API_PROVIDER_INCLUDE(extui)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ��������� ����������������������������������������()
	inline void InstallCryptoExtension() { ESB_INVOKE_API_PROC0(extui, InstallCryptoExtension); };
	// ��������� ���������������������������������������������([�����������������������������_ ��� ������������������])
	inline void BeginInstallCryptoExtension(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_ = DefUndef<NotifyDescription>) { ESB_INVOKE_API_PROC1(extui, BeginInstallCryptoExtension, NotifyDescriptionOnCompletion_); };
	// ������� ����������������������������������������() ���������� ������
	inline Boolean AttachCryptoExtension() { ESB_INVOKE_API_FUNC0(Boolean, extui, AttachCryptoExtension); };
	// ��������� �����������������������������������������������(������������������_ ��� ������������������)
	inline void BeginAttachingCryptoExtension(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(extui, BeginAttachingCryptoExtension, NotifyDescription_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void InstallCryptoExtension()
	inline void ����������������������������������������() { ESB_INVOKE_API_PROC0(extui, InstallCryptoExtension); };
	// Void BeginInstallCryptoExtension(NotifyDescription NotifyDescriptionOnCompletion_ = <default>)
	inline void ���������������������������������������������(����������<������������������> �����������������������������_ = ��������������������������<������������������>) { ESB_INVOKE_API_PROC1(extui, BeginInstallCryptoExtension, �����������������������������_); };
	// Function AttachCryptoExtension() -> Boolean
	inline ������ ����������������������������������������() { ESB_INVOKE_API_FUNC0(������, extui, AttachCryptoExtension); };
	// Void BeginAttachingCryptoExtension(NotifyDescription NotifyDescription_)
	inline void �����������������������������������������������(����������<������������������> ������������������_) { ESB_INVOKE_API_PROC1(extui, BeginAttachingCryptoExtension, ������������������_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(extui)
#if ESB_API_PROVIDER_INCLUDE(xml2)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ������� ������������������������XML(��������������_ ��� ������, [�������������_ ��� �����], [������_ ��� ������]) ���������� �����
	inline Number FindDisallowedXMLCharacters(ConstParam<String> CharacterString_, ConstParam<Number> StartPosition_ = DefUndef<Number>, ConstParam<String> Version_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(Number, xml2, FindDisallowedXMLCharacters, CharacterString_, StartPosition_, Version_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function FindDisallowedXMLCharacters(String CharacterString_, Number StartPosition_ = <default>, String Version_ = <default>) -> Number
	inline ����� ������������������������XML(����������<������> ��������������_, ����������<�����> �������������_ = ��������������������������<�����>, ����������<������> ������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC3(�����, xml2, FindDisallowedXMLCharacters, ��������������_, �������������_, ������_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(xml2)
#if ESB_API_PROVIDER_INCLUDE(mngcore)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ������� ���������������������������(�����_ ��� ������) ���������� ������
	inline Boolean IsTempStorageURL(ConstParam<String> Address_) { ESB_INVOKE_API_FUNC1(Boolean, mngcore, IsTempStorageURL, Address_); };
	// ������� ���������������������������������������������() ���������� ������
	inline String GetInfoBaseURL() { ESB_INVOKE_API_FUNC0(String, mngcore, GetInfoBaseURL); };
	// ������� ���������������������() ���������� ������
	inline String GetAllFilesMask() { ESB_INVOKE_API_FUNC0(String, mngcore, GetAllFilesMask); };
	// ������� �����������������������() ���������� ������
	inline String GetPathSeparator() { ESB_INVOKE_API_FUNC0(String, mngcore, GetPathSeparator); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function IsTempStorageURL(String Address_) -> Boolean
	inline ������ ���������������������������(����������<������> �����_) { ESB_INVOKE_API_FUNC1(������, mngcore, IsTempStorageURL, �����_); };
	// Function GetInfoBaseURL() -> String
	inline ������ ���������������������������������������������() { ESB_INVOKE_API_FUNC0(������, mngcore, GetInfoBaseURL); };
	// Function GetAllFilesMask() -> String
	inline ������ ���������������������() { ESB_INVOKE_API_FUNC0(������, mngcore, GetAllFilesMask); };
	// Function GetPathSeparator() -> String
	inline ������ �����������������������() { ESB_INVOKE_API_FUNC0(������, mngcore, GetPathSeparator); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(mngcore)
#if ESB_API_PROVIDER_INCLUDE(odata)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ��������� ��������������������������������������OData(�����������������_ ��� ������)
	inline void SetStandardODataInterfaceContent(ConstParam<Array> MetadataObjects_) { ESB_INVOKE_API_PROC1(odata, SetStandardODataInterfaceContent, MetadataObjects_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void SetStandardODataInterfaceContent(Array MetadataObjects_)
	inline void ��������������������������������������OData(����������<������> �����������������_) { ESB_INVOKE_API_PROC1(odata, SetStandardODataInterfaceContent, �����������������_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(odata)
#if ESB_API_PROVIDER_INCLUDE(core83)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ��������� ������������������������(��������_ ��� ������������, ��������_ ��� ������������, [�������������_ ��� ������], [����������������_ ��� ������])
	inline void FillPropertyValues(ConstParam<Arbitrary> Receiver_, ConstParam<Arbitrary> Source_, ConstParam<String> ListOfProperties_ = DefUndef<String>, ConstParam<String> ExcludeProperties_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(core83, FillPropertyValues, Receiver_, Source_, ListOfProperties_, ExcludeProperties_); };
	// ������� �����������������(��������_ ��� ������������) ���������� ������
	inline Boolean ValueIsFilled(ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_FUNC1(Boolean, core83, ValueIsFilled, Value_); };
	// ������� ��������(������_ ��� ������, ���������������_ ��� ������, [�����������������_ ��� �����������������], [����������������_ ��� �����], [��������������_ ��� �����]) ���������� �����
	inline Number StrFind(ConstParam<String> Line_, ConstParam<String> SearchSubstring_, ConstParam<SearchDirection> SearchDirection_ = SearchDirection::FromBegin, ConstParam<Number> StartIndex_ = DefUndef<Number>, ConstParam<Number> EntryNumber_ = Number::Value_1_) { ESB_INVOKE_API_FUNC5(Number, core83, StrFind, Line_, SearchSubstring_, SearchDirection_, StartIndex_, EntryNumber_); };
	// ������� ���������(������_ ��� ������, [��������1-��������10_ ��� ������������] ...) ���������� ������
	// !> The number of parameters differs from the disp-information:
	// !> Finction StrTemplate(_in_out_ _0, _in_out_ _1 = <Undef>, _in_out_ _2 = <Undef>, _in_out_ _3 = <Undef>, _in_out_ _4 = <Undef>, _in_out_ _5 = <Undef>, _in_out_ _6 = <Undef>, _in_out_ _7 = <Undef>, _in_out_ _8 = <Undef>, _in_out_ _9 = <Undef>, _in_out_ _10 = <Undef>); #5
	// ������� �����������(������1_ ��� ������, ������2_ ��� ������) ���������� �����
	inline Number StrCompare(ConstParam<String> String1_, ConstParam<String> String2_) { ESB_INVOKE_API_FUNC2(Number, core83, StrCompare, String1_, String2_); };
	// ������� ��������������(������_ ��� ������, ������������_ ��� ������) ���������� ������
	inline Boolean StrStartWith(ConstParam<String> String_, ConstParam<String> SearchString_) { ESB_INVOKE_API_FUNC2(Boolean, core83, StrStartWith, String_, SearchString_); };
	// ������� ������������������(������_ ��� ������, ������������_ ��� ������) ���������� ������
	inline Boolean StrEndsWith(ConstParam<String> String_, ConstParam<String> SearchString_) { ESB_INVOKE_API_FUNC2(Boolean, core83, StrEndsWith, String_, SearchString_); };
	// ������� ������������(������_ ��� ������, �����������_ ��� ������, [��������������_ ��� ������]) ���������� ��������<������>
	inline ArrayOf<String> StrSplit(ConstParam<String> String_, ConstParam<String> Separator_, ConstParam<Boolean> IncludeBlank_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC3(ArrayOf<String>, core83, StrSplit, String_, Separator_, IncludeBlank_); };
	// ������� ������������(������_ ��� ������<������, �������������������>, [�����������_ ��� ������]) ���������� ������
	inline String StrConcat(ConstParam<Array> Strings_, ConstParam<String> Separator_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, core83, StrConcat, Strings_, Separator_); };
	inline String StrConcat(ConstParam<FixedArray> Strings_, ConstParam<String> Separator_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, core83, StrConcat, Strings_, Separator_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void FillPropertyValues(Arbitrary Receiver_, Arbitrary Source_, String ListOfProperties_ = <default>, String ExcludeProperties_ = <default>)
	inline void ������������������������(����������<������������> ��������_, ����������<������������> ��������_, ����������<������> �������������_ = ��������������������������<������>, ����������<������> ����������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC4(core83, FillPropertyValues, ��������_, ��������_, �������������_, ����������������_); };
	// Function ValueIsFilled(Arbitrary Value_) -> Boolean
	inline ������ �����������������(����������<������������> ��������_) { ESB_INVOKE_API_FUNC1(������, core83, ValueIsFilled, ��������_); };
	// Function StrFind(String Line_, String SearchSubstring_, SearchDirection SearchDirection_ = <default>, Number StartIndex_ = <default>, Number EntryNumber_ = <default>) -> Number
	inline ����� ��������(����������<������> ������_, ����������<������> ���������������_, ����������<�����������������> �����������������_ = ��������������������������<�����������������>, ����������<�����> ����������������_ = ��������������������������<�����>, ����������<�����> ��������������_ = ��������������������������<�����>) { ESB_INVOKE_API_FUNC5(�����, core83, StrFind, ������_, ���������������_, �����������������_, ����������������_, ��������������_); };
	// Function StrTemplate(String Template_, Arbitrary Value1-Value10_ = <default> ...) -> String
	// !> The number of parameters differs from the disp-information:
	// !> Finction StrTemplate(_in_out_ _0, _in_out_ _1 = <Undef>, _in_out_ _2 = <Undef>, _in_out_ _3 = <Undef>, _in_out_ _4 = <Undef>, _in_out_ _5 = <Undef>, _in_out_ _6 = <Undef>, _in_out_ _7 = <Undef>, _in_out_ _8 = <Undef>, _in_out_ _9 = <Undef>, _in_out_ _10 = <Undef>); #5
	// Function StrCompare(String String1_, String String2_) -> Number
	inline ����� �����������(����������<������> ������1_, ����������<������> ������2_) { ESB_INVOKE_API_FUNC2(�����, core83, StrCompare, ������1_, ������2_); };
	// Function StrStartWith(String String_, String SearchString_) -> Boolean
	inline ������ ��������������(����������<������> ������_, ����������<������> ������������_) { ESB_INVOKE_API_FUNC2(������, core83, StrStartWith, ������_, ������������_); };
	// Function StrEndsWith(String String_, String SearchString_) -> Boolean
	inline ������ ������������������(����������<������> ������_, ����������<������> ������������_) { ESB_INVOKE_API_FUNC2(������, core83, StrEndsWith, ������_, ������������_); };
	// Function StrSplit(String String_, String Separator_, Boolean IncludeBlank_ = <default>) -> ArrayOf<String>
	inline ��������<������> ������������(����������<������> ������_, ����������<������> �����������_, ����������<������> ��������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC3(��������<������>, core83, StrSplit, ������_, �����������_, ��������������_); };
	// Function StrConcat(OneOf<Array, FixedArray> Strings_, String Separator_ = <default>) -> String
	inline ������ ������������(����������<������> ������_, ����������<������> �����������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC2(������, core83, StrConcat, ������_, �����������_); };
	inline ������ ������������(����������<�������������������> ������_, ����������<������> �����������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC2(������, core83, StrConcat, ������_, �����������_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(core83)
#if ESB_API_PROVIDER_INCLUDE(backend0x7)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ��������� �����������������������()
	inline void CommitTransaction() { ESB_INVOKE_API_PROC0(backend0x7, CommitTransaction); };
	// ��������� ������������������()
	inline void RollbackTransaction() { ESB_INVOKE_API_PROC0(backend0x7, RollbackTransaction); };
	// ������� �����������������() ���������� ������
	inline Boolean TransactionActive() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, TransactionActive); };
	// ��������� ��������(��������������_ ��� ������, [������_ ��� ���������������])
	inline void Message(ConstParam<String> MessageText_, ConstParam<MessageStatus> Status_ = DefUndef<MessageStatus>) { ESB_INVOKE_API_PROC2(backend0x7, Message, MessageText_, Status_); };
	// ��������� ��������������������������(����������������_ ��� ������)
	inline void SetExclusiveMode(ConstParam<Boolean> ExclusiveMode_) { ESB_INVOKE_API_PROC1(backend0x7, SetExclusiveMode, ExclusiveMode_); };
	// ������� ����������������() ���������� ������
	inline Boolean ExclusiveMode() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, ExclusiveMode); };
	// ������� ����(��������������_ ��� ������, [��������_ ��� ������]) ���������� ������
	inline String NStr(ConstParam<String> InitialString_, ConstParam<String> LanguageCode_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(String, backend0x7, NStr, InitialString_, LanguageCode_); };
	// ������� ���������������������������������() ���������� ����
	inline DateTime GetRealTimeTimestamp() { ESB_INVOKE_API_FUNC0(DateTime, backend0x7, GetRealTimeTimestamp); };
	// ������� ��������������������(��������_ ��� ������������) ���������� ������
	inline String ValueToStringInternal(ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_FUNC1(String, backend0x7, ValueToStringInternal, Value_); };
	// ������� ���������������������(������_ ��� ������) ���������� ������������
	inline Arbitrary ValueFromStringInternal(ConstParam<String> Line_) { ESB_INVOKE_API_FUNC1(Arbitrary, backend0x7, ValueFromStringInternal, Line_); };
	// ������� �������������(��������_ ��� ������, ��������_ ��� ������������) ���������� ������
	inline Boolean ValueToFile(ConstParam<String> FileName_, ConstParam<Arbitrary> Value_) { ESB_INVOKE_API_FUNC2(Boolean, backend0x7, ValueToFile, FileName_, Value_); };
	// ������� ���������������(��������_ ��� ������) ���������� ������������
	inline Arbitrary ValueFromFile(ConstParam<String> FileName_) { ESB_INVOKE_API_FUNC1(Arbitrary, backend0x7, ValueFromFile, FileName_); };
	// ������� �������������() ���������� ������
	inline String ComputerName() { ESB_INVOKE_API_FUNC0(String, backend0x7, ComputerName); };
	// ������� ���������������() ���������� ������
	inline String UserName() { ESB_INVOKE_API_FUNC0(String, backend0x7, UserName); };
	// ������� ���������������������() ���������� ������
	inline String UserFullName() { ESB_INVOKE_API_FUNC0(String, backend0x7, UserFullName); };
	// ������� ����������������() ���������� ������
	inline String BinDir() { ESB_INVOKE_API_FUNC0(String, backend0x7, BinDir); };
	// ������� ����������������������() ���������� ������
	inline String TempFilesDir() { ESB_INVOKE_API_FUNC0(String, backend0x7, TempFilesDir); };
	// ��������� ��������������������������������������(������������������_ ��� ������������������)
	inline void BeginGettingTempFilesDir(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(backend0x7, BeginGettingTempFilesDir, NotifyDescription_); };
	// ������� ���������������������() ���������� ������
	inline String CurrentLocaleCode() { ESB_INVOKE_API_FUNC0(String, backend0x7, CurrentLocaleCode); };
	// ������� ��������������������������������() ���������� ������
	inline String InfoBaseLocaleCode() { ESB_INVOKE_API_FUNC0(String, backend0x7, InfoBaseLocaleCode); };
	// ������� ����������������������������������() ���������� ������
	inline String InfoBaseConnectionString() { ESB_INVOKE_API_FUNC0(String, backend0x7, InfoBaseConnectionString); };
	// ��������� ����������������������������([����������������_ ��� ������], [�����������������_ ��� ������], ������_ ��� ���������)
	inline void CopyEventLog(ConstParam<UndefOr<String>> InputFileName_, ConstParam<UndefOr<String>> OutputFileName_, ConstParam<Structure> Filter_) { ESB_INVOKE_API_PROC3(backend0x7, CopyEventLog, InputFileName_, OutputFileName_, Filter_); };
	// ��������� �������������������������(������_ ��� ���������)
	inline void ClearEventLog(ConstParam<Structure> Filter_) { ESB_INVOKE_API_PROC1(backend0x7, ClearEventLog, Filter_); };
	// ������� �����������������������(�������������_ ��� ������) ���������� ������
	inline String ApplicationPresentation(ConstParam<String> ApplicationIdentifier_) { ESB_INVOKE_API_FUNC1(String, backend0x7, ApplicationPresentation, ApplicationIdentifier_); };
	// ������� ���������������������������������() ���������� �����
	inline Number InfoBaseConnectionNumber() { ESB_INVOKE_API_FUNC0(Number, backend0x7, InfoBaseConnectionNumber); };
	// ������� �����������������������������() ���������� �����
	inline Number InfoBaseSessionNumber() { ESB_INVOKE_API_FUNC0(Number, backend0x7, InfoBaseSessionNumber); };
	// ������� ��������������������������������������(��������������������_ ��� ������) ���������� ������
	inline String EventLogEventPresentation(ConstParam<String> EventIdentifier_) { ESB_INVOKE_API_FUNC1(String, backend0x7, EventLogEventPresentation, EventIdentifier_); };
	// ��������� �����������������������������������������(������_ ��� ������)
	inline void SetEventLogUsing(ConstParam<Array> Levels_) { ESB_INVOKE_API_PROC1(backend0x7, SetEventLogUsing, Levels_); };
	// ������� ��������������������() ���������� ������
	inline Boolean ConfigurationChanged() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, ConfigurationChanged); };
	// ������� �����������������������������������������() ���������� ������
	inline Boolean DataBaseConfigurationChangedDynamically() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, DataBaseConfigurationChangedDynamically); };
	// ��������� ���������������������������������������([�������_ ��� �����])
	inline void SetLockWaitTime(ConstParam<Number> Timeout_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(backend0x7, SetLockWaitTime, Timeout_); };
	// ������� �������������������������������������() ���������� �����
	inline Number GetLockWaitTime() { ESB_INVOKE_API_FUNC0(Number, backend0x7, GetLockWaitTime); };
	// ��������� ����������������������������������������������([�����_ ��� �����])
	inline void SetUserPasswordMinLength(ConstParam<Number> Length_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(backend0x7, SetUserPasswordMinLength, Length_); };
	// ������� ��������������������������������������������() ���������� �����
	inline Number GetUserPasswordMinLength() { ESB_INVOKE_API_FUNC0(Number, backend0x7, GetUserPasswordMinLength); };
	// ��������� �����������������������������������������������(�������������_ ��� ������)
	inline void SetUserPasswordStrengthCheck(ConstParam<Boolean> ComplexPassword_) { ESB_INVOKE_API_PROC1(backend0x7, SetUserPasswordStrengthCheck, ComplexPassword_); };
	// ������� ���������������������������������������������() ���������� ������
	inline Boolean GetUserPasswordStrengthCheck() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, GetUserPasswordStrengthCheck); };
	// ������� ������������������() ���������� ������
	inline String CurrentSystemLanguage() { ESB_INVOKE_API_FUNC0(String, backend0x7, CurrentSystemLanguage); };
	// ������� ��������������������������(������������������_ ��� ������������������) ���������� ������
	inline String BriefErrorDescription(ConstParam<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_FUNC1(String, backend0x7, BriefErrorDescription, ErrorInfo_); };
	// ������� ����������������������������(������������������_ ��� ������������������) ���������� ������
	inline String DetailErrorDescription(ConstParam<ErrorInfoValue> ErrorInfo_) { ESB_INVOKE_API_FUNC1(String, backend0x7, DetailErrorDescription, ErrorInfo_); };
	// ��������� ��������������������������������(��������_ ��� ������)
	inline void SetPrivilegedMode(ConstParam<Boolean> PrivilegedModeOn_) { ESB_INVOKE_API_PROC1(backend0x7, SetPrivilegedMode, PrivilegedModeOn_); };
	// ������� ����������������������() ���������� ������
	inline Boolean PrivilegedMode() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, PrivilegedMode); };
	// ��������� ������������������������������������(����_ ��� ������������, [������������_ ��� ������], [������������������_ ��� �����������������������])
	inline void LockDataForEdit(ConstParam<Arbitrary> Key_, ConstParam<String> DataVersion_ = DefUndef<String>, ConstParam<UUIDValue> FormIdentifier_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_PROC3(backend0x7, LockDataForEdit, Key_, DataVersion_, FormIdentifier_); };
	// ��������� ������������������������������������()
	inline void RefreshReusableValues() { ESB_INVOKE_API_PROC0(backend0x7, RefreshReusableValues); };
	// ������� ����������������������������������������([�������_ ��� ������], [����������������_ ��� ������]) ���������� ���������_GETEVENTLOGFILTERVALUES
	//TODO	Structure_GETEVENTLOGFILTERVALUES
	inline Structure GetEventLogFilterValues(ConstParam<String> Columns_ = DefUndef<String>, ConstParam<String> InputFileName_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(Structure, backend0x7, GetEventLogFilterValues, Columns_, InputFileName_); };
	// ��������� �������������������������(���������������_ ��� ������<������, ������>)
	inline void SetSafeMode(ConstParam<Boolean> SafeMode_) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeMode, SafeMode_); };
	inline void SetSafeMode(ConstParam<String> SafeMode_) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeMode, SafeMode_); };
	// ������� ���������������() ���������� ������<������, ������>
	inline OneOf<Boolean, String> SafeMode() { ESB_INVOKE_API_FUNC0(ESB_ONEOF(Boolean, String), backend0x7, SafeMode); };
	// ��������� ���������������������������������������([�����������_ ��� ������])
	inline void SetInfoBaseTimeZone(ConstParam<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_PROC1(backend0x7, SetInfoBaseTimeZone, TimeZone_); };
	// ������� �������������������������������������() ���������� ������<������, ������������>
	inline UndefOr<String> GetInfoBaseTimeZone() { ESB_INVOKE_API_FUNC0(UndefOr<String>, backend0x7, GetInfoBaseTimeZone); };
	// ��������� ���������������������������([�����������_ ��� ������])
	inline void SetSessionTimeZone(ConstParam<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_PROC1(backend0x7, SetSessionTimeZone, TimeZone_); };
	// ������� �����������������() ���������� ������
	inline String SessionTimeZone() { ESB_INVOKE_API_FUNC0(String, backend0x7, SessionTimeZone); };
	// ������� �����������������() ���������� ����
	inline DateTime CurrentSessionDate() { ESB_INVOKE_API_FUNC0(DateTime, backend0x7, CurrentSessionDate); };
	// ������� ����������������������([�����������_ ��� ������<������, ������������>], [������������������_ ��� ������<����, ������������>]) ���������� �����
	inline Number DaylightTimeOffset(ConstParam<String> TimeZone_ = DefUndef<String>, ConstParam<DateTime> UniversalTime_ = DefUndef<DateTime>) { ESB_INVOKE_API_FUNC2(Number, backend0x7, DaylightTimeOffset, TimeZone_, UniversalTime_); };
	// ������� ������������������������() ���������� ����
	inline DateTime CurrentUniversalDate() { ESB_INVOKE_API_FUNC0(DateTime, backend0x7, CurrentUniversalDate); };
	// ������� ������������(������������������_ ��� ����, [�����������_ ��� ������<������, ������������>]) ���������� ����
	inline DateTime ToLocalTime(ConstParam<DateTime> UniversalTime_, ConstParam<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(DateTime, backend0x7, ToLocalTime, UniversalTime_, TimeZone_); };
	// ������� ������������������(������������_ ��� ����, [�����������_ ��� ������<������, ������������>]) ���������� ����
	inline DateTime ToUniversalTime(ConstParam<DateTime> LocalTime_, ConstParam<String> TimeZone_ = DefUndef<String>) { ESB_INVOKE_API_FUNC2(DateTime, backend0x7, ToUniversalTime, LocalTime_, TimeZone_); };
	// ������� �����������() ���������� ������
	inline String TimeZone() { ESB_INVOKE_API_FUNC0(String, backend0x7, TimeZone); };
	// ������� ������������������������������() ���������� ��������<������>
	inline ArrayOf<String> GetAvailableTimeZones() { ESB_INVOKE_API_FUNC0(ArrayOf<String>, backend0x7, GetAvailableTimeZones); };
	// ������� ��������������������������(�����������_ ��� ������) ���������� ������
	inline String TimeZonePresentation(ConstParam<String> TimeZone_) { ESB_INVOKE_API_FUNC1(String, backend0x7, TimeZonePresentation, TimeZone_); };
	// ������� ���������������������������������() ���������� ��������<������>
	inline ArrayOf<String> GetAvailableLocaleCodes() { ESB_INVOKE_API_FUNC0(ArrayOf<String>, backend0x7, GetAvailableLocaleCodes); };
	// ������� ����������������������������(��������������_ ��� ������) ���������� ������
	inline String LocaleCodePresentation(ConstParam<String> LocalizationCode_) { ESB_INVOKE_API_FUNC1(String, backend0x7, LocaleCodePresentation, LocalizationCode_); };
	// ������� ���������������������������([�����������_ ��� ������<������, ������������>], [������������������_ ��� ������<����, ������������>]) ���������� ����
	inline DateTime StandardTimeOffset(ConstParam<String> TimeZone_ = DefUndef<String>, ConstParam<DateTime> UniversalTime_ = DefUndef<DateTime>) { ESB_INVOKE_API_FUNC2(DateTime, backend0x7, StandardTimeOffset, TimeZone_, UniversalTime_); };
	// ������� ��������������������������([����������_ ��� ������]) ���������� ������
	inline String GetTempFileName(ConstParam<String> Extension_ = DefUndef<String>) { ESB_INVOKE_API_FUNC1(String, backend0x7, GetTempFileName, Extension_); };
	// ������� ������������������([��������_ ��� ������]) ���������� ������
	inline String RightPresentation(ConstParam<String> NameOfRight_ = DefUndef<String>) { ESB_INVOKE_API_FUNC1(String, backend0x7, RightPresentation, NameOfRight_); };
	// ��������� �������������������������������()
	inline void EraseInfoBaseData() { ESB_INVOKE_API_PROC0(backend0x7, EraseInfoBaseData); };
	// ��������� �����������������������������������������(�������������_ ��� ������, �����_ ��� ������)
	inline void SetDataSeparationSafeMode(ConstParam<String> CommonAttribute_, ConstParam<Boolean> Mode_) { ESB_INVOKE_API_PROC2(backend0x7, SetDataSeparationSafeMode, CommonAttribute_, Mode_); };
	// ������� �������������������������������(�������������_ ��� ������) ���������� ������
	inline Boolean DataSeparationSafeMode(ConstParam<String> CommonAttribute_) { ESB_INVOKE_API_FUNC1(Boolean, backend0x7, DataSeparationSafeMode, CommonAttribute_); };
	// ��������� ��������������������������������������()
	inline void InitializePredefinedData() { ESB_INVOKE_API_PROC0(backend0x7, InitializePredefinedData); };
	// ��������� ����������������������������������������([�������_ ��� �����])
	inline void SetPassiveSessionHibernateTime(ConstParam<Number> Timeout_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(backend0x7, SetPassiveSessionHibernateTime, Timeout_); };
	// ������� ��������������������������������������() ���������� �����
	inline Number GetPassiveSessionHibernateTime() { ESB_INVOKE_API_FUNC0(Number, backend0x7, GetPassiveSessionHibernateTime); };
	// ��������� ��������������������������������������([�������_ ��� �����])
	inline void SetHibernateSessionTerminateTime(ConstParam<Number> Timeout_ = DefUndef<Number>) { ESB_INVOKE_API_PROC1(backend0x7, SetHibernateSessionTerminateTime, Timeout_); };
	// ������� ������������������������������������() ���������� �����
	inline Number GetHibernateSessionTerminateTime() { ESB_INVOKE_API_FUNC0(Number, backend0x7, GetHibernateSessionTerminateTime); };
	// ��������� ����������������������������������������([���_ ��� ������], [������_ ��� ������], [����������������������_ ��� ������])
	inline void SetLicensingClientParameters(ConstParam<String> Name_ = DefUndef<String>, ConstParam<String> Password_ = DefUndef<String>, ConstParam<String> AdditionalOption_ = DefUndef<String>) { ESB_INVOKE_API_PROC3(backend0x7, SetLicensingClientParameters, Name_, Password_, AdditionalOption_); };
	// ��������� �������������������������������������([���������_ ��� ������])
	inline void SetSafeModeDisabled(ConstParam<Boolean> Disable_ = DefUndef<Boolean>) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeModeDisabled, Disable_); };
	// ������� �����������������������������������() ���������� ������
	inline Boolean GetSafeModeDisabled() { ESB_INVOKE_API_FUNC0(Boolean, backend0x7, GetSafeModeDisabled); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void CommitTransaction()
	inline void �����������������������() { ESB_INVOKE_API_PROC0(backend0x7, CommitTransaction); };
	// Void RollbackTransaction()
	inline void ������������������() { ESB_INVOKE_API_PROC0(backend0x7, RollbackTransaction); };
	// Function TransactionActive() -> Boolean
	inline ������ �����������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, TransactionActive); };
	// Void Message(String MessageText_, MessageStatus Status_ = <default>)
	inline void ��������(����������<������> ��������������_, ����������<���������������> ������_ = ��������������������������<���������������>) { ESB_INVOKE_API_PROC2(backend0x7, Message, ��������������_, ������_); };
	// Void SetExclusiveMode(Boolean ExclusiveMode_)
	inline void ��������������������������(����������<������> ����������������_) { ESB_INVOKE_API_PROC1(backend0x7, SetExclusiveMode, ����������������_); };
	// Function ExclusiveMode() -> Boolean
	inline ������ ����������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, ExclusiveMode); };
	// Function NStr(String InitialString_, String LanguageCode_ = <default>) -> String
	inline ������ ����(����������<������> ��������������_, ����������<������> ��������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC2(������, backend0x7, NStr, ��������������_, ��������_); };
	// Function GetRealTimeTimestamp() -> Date
	inline ��������� ���������������������������������() { ESB_INVOKE_API_FUNC0(���������, backend0x7, GetRealTimeTimestamp); };
	// Function ValueToStringInternal(Arbitrary Value_) -> String
	inline ������ ��������������������(����������<������������> ��������_) { ESB_INVOKE_API_FUNC1(������, backend0x7, ValueToStringInternal, ��������_); };
	// Function ValueFromStringInternal(String Line_) -> Arbitrary
	inline ������������ ���������������������(����������<������> ������_) { ESB_INVOKE_API_FUNC1(������������, backend0x7, ValueFromStringInternal, ������_); };
	// Function ValueToFile(String FileName_, Arbitrary Value_) -> Boolean
	inline ������ �������������(����������<������> ��������_, ����������<������������> ��������_) { ESB_INVOKE_API_FUNC2(������, backend0x7, ValueToFile, ��������_, ��������_); };
	// Function ValueFromFile(String FileName_) -> Arbitrary
	inline ������������ ���������������(����������<������> ��������_) { ESB_INVOKE_API_FUNC1(������������, backend0x7, ValueFromFile, ��������_); };
	// Function ComputerName() -> String
	inline ������ �������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, ComputerName); };
	// Function UserName() -> String
	inline ������ ���������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, UserName); };
	// Function UserFullName() -> String
	inline ������ ���������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, UserFullName); };
	// Function BinDir() -> String
	inline ������ ����������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, BinDir); };
	// Function TempFilesDir() -> String
	inline ������ ����������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, TempFilesDir); };
	// Void BeginGettingTempFilesDir(NotifyDescription NotifyDescription_)
	inline void ��������������������������������������(����������<������������������> ������������������_) { ESB_INVOKE_API_PROC1(backend0x7, BeginGettingTempFilesDir, ������������������_); };
	// Function CurrentLocaleCode() -> String
	inline ������ ���������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, CurrentLocaleCode); };
	// Function InfoBaseLocaleCode() -> String
	inline ������ ��������������������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, InfoBaseLocaleCode); };
	// Function InfoBaseConnectionString() -> String
	inline ������ ����������������������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, InfoBaseConnectionString); };
	// Void CopyEventLog(String InputFileName_ = <default>, String OutputFileName_ = <default>, Structure Filter_)
	inline void ����������������������������(����������<���������������<������>> ����������������_, ����������<���������������<������>> �����������������_, ����������<���������> ������_) { ESB_INVOKE_API_PROC3(backend0x7, CopyEventLog, ����������������_, �����������������_, ������_); };
	// Void ClearEventLog(Structure Filter_)
	inline void �������������������������(����������<���������> ������_) { ESB_INVOKE_API_PROC1(backend0x7, ClearEventLog, ������_); };
	// Function ApplicationPresentation(String ApplicationIdentifier_) -> String
	inline ������ �����������������������(����������<������> �������������_) { ESB_INVOKE_API_FUNC1(������, backend0x7, ApplicationPresentation, �������������_); };
	// Function InfoBaseConnectionNumber() -> Number
	inline ����� ���������������������������������() { ESB_INVOKE_API_FUNC0(�����, backend0x7, InfoBaseConnectionNumber); };
	// Function InfoBaseSessionNumber() -> Number
	inline ����� �����������������������������() { ESB_INVOKE_API_FUNC0(�����, backend0x7, InfoBaseSessionNumber); };
	// Function EventLogEventPresentation(String EventIdentifier_) -> String
	inline ������ ��������������������������������������(����������<������> ��������������������_) { ESB_INVOKE_API_FUNC1(������, backend0x7, EventLogEventPresentation, ��������������������_); };
	// Void SetEventLogUsing(Array Levels_)
	inline void �����������������������������������������(����������<������> ������_) { ESB_INVOKE_API_PROC1(backend0x7, SetEventLogUsing, ������_); };
	// Function ConfigurationChanged() -> Boolean
	inline ������ ��������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, ConfigurationChanged); };
	// Function DataBaseConfigurationChangedDynamically() -> Boolean
	inline ������ �����������������������������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, DataBaseConfigurationChangedDynamically); };
	// Void SetLockWaitTime(Number Timeout_ = <default>)
	inline void ���������������������������������������(����������<�����> �������_ = ��������������������������<�����>) { ESB_INVOKE_API_PROC1(backend0x7, SetLockWaitTime, �������_); };
	// Function GetLockWaitTime() -> Number
	inline ����� �������������������������������������() { ESB_INVOKE_API_FUNC0(�����, backend0x7, GetLockWaitTime); };
	// Void SetUserPasswordMinLength(Number Length_ = <default>)
	inline void ����������������������������������������������(����������<�����> �����_ = ��������������������������<�����>) { ESB_INVOKE_API_PROC1(backend0x7, SetUserPasswordMinLength, �����_); };
	// Function GetUserPasswordMinLength() -> Number
	inline ����� ��������������������������������������������() { ESB_INVOKE_API_FUNC0(�����, backend0x7, GetUserPasswordMinLength); };
	// Void SetUserPasswordStrengthCheck(Boolean ComplexPassword_)
	inline void �����������������������������������������������(����������<������> �������������_) { ESB_INVOKE_API_PROC1(backend0x7, SetUserPasswordStrengthCheck, �������������_); };
	// Function GetUserPasswordStrengthCheck() -> Boolean
	inline ������ ���������������������������������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, GetUserPasswordStrengthCheck); };
	// Function CurrentSystemLanguage() -> String
	inline ������ ������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, CurrentSystemLanguage); };
	// Function BriefErrorDescription(ErrorInfo ErrorInfo_) -> String
	inline ������ ��������������������������(����������<������������������> ������������������_) { ESB_INVOKE_API_FUNC1(������, backend0x7, BriefErrorDescription, ������������������_); };
	// Function DetailErrorDescription(ErrorInfo ErrorInfo_) -> String
	inline ������ ����������������������������(����������<������������������> ������������������_) { ESB_INVOKE_API_FUNC1(������, backend0x7, DetailErrorDescription, ������������������_); };
	// Void SetPrivilegedMode(Boolean PrivilegedModeOn_)
	inline void ��������������������������������(����������<������> ��������_) { ESB_INVOKE_API_PROC1(backend0x7, SetPrivilegedMode, ��������_); };
	// Function PrivilegedMode() -> Boolean
	inline ������ ����������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, PrivilegedMode); };
	// Void LockDataForEdit(Arbitrary Key_, String DataVersion_ = <default>, UUID FormIdentifier_ = <default>)
	inline void ������������������������������������(����������<������������> ����_, ����������<������> ������������_ = ��������������������������<������>, ����������<�����������������������> ������������������_ = ��������������������������<�����������������������>) { ESB_INVOKE_API_PROC3(backend0x7, LockDataForEdit, ����_, ������������_, ������������������_); };
	// Void RefreshReusableValues()
	inline void ������������������������������������() { ESB_INVOKE_API_PROC0(backend0x7, RefreshReusableValues); };
	// Function GetEventLogFilterValues(String Columns_ = <default>, String InputFileName_ = <default>) -> Structure_GETEVENTLOGFILTERVALUES
	//TODO	���������_GETEVENTLOGFILTERVALUES
	inline ��������� ����������������������������������������(����������<������> �������_ = ��������������������������<������>, ����������<������> ����������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC2(���������, backend0x7, GetEventLogFilterValues, �������_, ����������������_); };
	// Void SetSafeMode(OneOf<Boolean, String> SafeMode_)
	inline void �������������������������(����������<������> ���������������_) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeMode, ���������������_); };
	inline void �������������������������(����������<������> ���������������_) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeMode, ���������������_); };
	// Function SafeMode() -> OneOf<Boolean, String>
	inline ������<������, ������> ���������������() { ESB_INVOKE_API_FUNC0(ESB_ONEOF(������, ������), backend0x7, SafeMode); };
	// Void SetInfoBaseTimeZone(String TimeZone_ = <default>)
	inline void ���������������������������������������(����������<������> �����������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC1(backend0x7, SetInfoBaseTimeZone, �����������_); };
	// Function GetInfoBaseTimeZone() -> OneOf<String, Undefined>
	inline ���������������<������> �������������������������������������() { ESB_INVOKE_API_FUNC0(���������������<������>, backend0x7, GetInfoBaseTimeZone); };
	// Void SetSessionTimeZone(String TimeZone_ = <default>)
	inline void ���������������������������(����������<������> �����������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC1(backend0x7, SetSessionTimeZone, �����������_); };
	// Function SessionTimeZone() -> String
	inline ������ �����������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, SessionTimeZone); };
	// Function CurrentSessionDate() -> Date
	inline ��������� �����������������() { ESB_INVOKE_API_FUNC0(���������, backend0x7, CurrentSessionDate); };
	// Function DaylightTimeOffset(OneOf<String, Undefined> TimeZone_ = <default>, OneOf<Date, Undefined> UniversalTime_ = <default>) -> Number
	inline ����� ����������������������(����������<������> �����������_ = ��������������������������<������>, ����������<���������> ������������������_ = ��������������������������<���������>) { ESB_INVOKE_API_FUNC2(�����, backend0x7, DaylightTimeOffset, �����������_, ������������������_); };
	// Function CurrentUniversalDate() -> Date
	inline ��������� ������������������������() { ESB_INVOKE_API_FUNC0(���������, backend0x7, CurrentUniversalDate); };
	// Function ToLocalTime(Date UniversalTime_, OneOf<String, Undefined> TimeZone_ = <default>) -> Date
	inline ��������� ������������(����������<���������> ������������������_, ����������<������> �����������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC2(���������, backend0x7, ToLocalTime, ������������������_, �����������_); };
	// Function ToUniversalTime(Date LocalTime_, OneOf<String, Undefined> TimeZone_ = <default>) -> Date
	inline ��������� ������������������(����������<���������> ������������_, ����������<������> �����������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC2(���������, backend0x7, ToUniversalTime, ������������_, �����������_); };
	// Function TimeZone() -> String
	inline ������ �����������() { ESB_INVOKE_API_FUNC0(������, backend0x7, TimeZone); };
	// Function GetAvailableTimeZones() -> ArrayOf<String>
	inline ��������<������> ������������������������������() { ESB_INVOKE_API_FUNC0(��������<������>, backend0x7, GetAvailableTimeZones); };
	// Function TimeZonePresentation(String TimeZone_) -> String
	inline ������ ��������������������������(����������<������> �����������_) { ESB_INVOKE_API_FUNC1(������, backend0x7, TimeZonePresentation, �����������_); };
	// Function GetAvailableLocaleCodes() -> ArrayOf<String>
	inline ��������<������> ���������������������������������() { ESB_INVOKE_API_FUNC0(��������<������>, backend0x7, GetAvailableLocaleCodes); };
	// Function LocaleCodePresentation(String LocalizationCode_) -> String
	inline ������ ����������������������������(����������<������> ��������������_) { ESB_INVOKE_API_FUNC1(������, backend0x7, LocaleCodePresentation, ��������������_); };
	// Function StandardTimeOffset(OneOf<String, Undefined> TimeZone_ = <default>, OneOf<Date, Undefined> UniversalTime_ = <default>) -> Date
	inline ��������� ���������������������������(����������<������> �����������_ = ��������������������������<������>, ����������<���������> ������������������_ = ��������������������������<���������>) { ESB_INVOKE_API_FUNC2(���������, backend0x7, StandardTimeOffset, �����������_, ������������������_); };
	// Function GetTempFileName(String Extension_ = <default>) -> String
	inline ������ ��������������������������(����������<������> ����������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC1(������, backend0x7, GetTempFileName, ����������_); };
	// Function RightPresentation(String NameOfRight_ = <default>) -> String
	inline ������ ������������������(����������<������> ��������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC1(������, backend0x7, RightPresentation, ��������_); };
	// Void EraseInfoBaseData()
	inline void �������������������������������() { ESB_INVOKE_API_PROC0(backend0x7, EraseInfoBaseData); };
	// Void SetDataSeparationSafeMode(String CommonAttribute_, Boolean Mode_)
	inline void �����������������������������������������(����������<������> �������������_, ����������<������> �����_) { ESB_INVOKE_API_PROC2(backend0x7, SetDataSeparationSafeMode, �������������_, �����_); };
	// Function DataSeparationSafeMode(String CommonAttribute_) -> Boolean
	inline ������ �������������������������������(����������<������> �������������_) { ESB_INVOKE_API_FUNC1(������, backend0x7, DataSeparationSafeMode, �������������_); };
	// Void InitializePredefinedData()
	inline void ��������������������������������������() { ESB_INVOKE_API_PROC0(backend0x7, InitializePredefinedData); };
	// Void SetPassiveSessionHibernateTime(Number Timeout_ = <default>)
	inline void ����������������������������������������(����������<�����> �������_ = ��������������������������<�����>) { ESB_INVOKE_API_PROC1(backend0x7, SetPassiveSessionHibernateTime, �������_); };
	// Function GetPassiveSessionHibernateTime() -> Number
	inline ����� ��������������������������������������() { ESB_INVOKE_API_FUNC0(�����, backend0x7, GetPassiveSessionHibernateTime); };
	// Void SetHibernateSessionTerminateTime(Number Timeout_ = <default>)
	inline void ��������������������������������������(����������<�����> �������_ = ��������������������������<�����>) { ESB_INVOKE_API_PROC1(backend0x7, SetHibernateSessionTerminateTime, �������_); };
	// Function GetHibernateSessionTerminateTime() -> Number
	inline ����� ������������������������������������() { ESB_INVOKE_API_FUNC0(�����, backend0x7, GetHibernateSessionTerminateTime); };
	// Void SetLicensingClientParameters(String Name_ = <default>, String Password_ = <default>, String AdditionalOption_ = <default>)
	inline void ����������������������������������������(����������<������> ���_ = ��������������������������<������>, ����������<������> ������_ = ��������������������������<������>, ����������<������> ����������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC3(backend0x7, SetLicensingClientParameters, ���_, ������_, ����������������������_); };
	// Void SetSafeModeDisabled(Boolean Disable_ = <default>)
	inline void �������������������������������������(����������<������> ���������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC1(backend0x7, SetSafeModeDisabled, ���������_); };
	// Function GetSafeModeDisabled() -> Boolean
	inline ������ �����������������������������������() { ESB_INVOKE_API_FUNC0(������, backend0x7, GetSafeModeDisabled); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(backend0x7)
#if ESB_API_PROVIDER_INCLUDE(addin)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ������� ���������������������������(��������������_ ��� ������, ���_ ��� ������, [���_ ��� ��������������������]) ���������� ������
	inline Boolean AttachAddIn(ConstParam<String> Location_, ConstParam<String> Name_, ConstParam<AddInType> type_ = DefUndef<AddInType>) { ESB_INVOKE_API_FUNC3(Boolean, addin, AttachAddIn, Location_, Name_, type_); };
	// ������� ���������������������������(��������������_ ��� ������, ���_ ��� ������, [���_ ��� ��������������������]) ���������� ������
	inline Boolean AttachAddIn(ConstParam<String> ObjectIdentificator_) { ESB_INVOKE_API_FUNC3(Boolean, addin, AttachAddIn, ObjectIdentificator_, ESB_DEFARG(Undefined), ESB_DEFARG(Undefined)); };
	// ��������� ��������������������������(��������_ ��� ������)
	inline void LoadAddIn(ConstParam<String> FileName_) { ESB_INVOKE_API_PROC1(addin, LoadAddIn, FileName_); };
	// ��������� ����������������������������������([������������������_ ��� ������������������], [��������������������_ ��� ������])
	inline void BeginAttachingAddIn(ConstParam<NotifyDescription> NotifyDescription_ = DefUndef<NotifyDescription>, ConstParam<String> ObjectId_ = DefUndef<String>) { ESB_INVOKE_API_PROC4(addin, BeginAttachingAddIn, NotifyDescription_, ObjectId_, ESB_DEFARG(Undefined), ESB_DEFARG(Undefined)); };
	// ��������� ����������������������������������([������������������_ ��� ������������������], [��������������������_ ��� ������])
	inline void BeginAttachingAddIn(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<String> Location_, ConstParam<String> Name_, ConstParam<AddInType> type_ = DefUndef<AddInType>) { ESB_INVOKE_API_PROC4(addin, BeginAttachingAddIn, NotifyDescription_, Location_, Name_, type_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function AttachAddIn(String Location_, String Name_, AddInType Type_ = <default>) -> Boolean
	inline ������ ���������������������������(����������<������> ��������������_, ����������<������> ���_, ����������<��������������������> ���_ = ��������������������������<��������������������>) { ESB_INVOKE_API_FUNC3(������, addin, AttachAddIn, ��������������_, ���_, ���_); };
	// Function AttachAddIn(String Location_, String Name_, AddInType Type_ = <default>) -> Boolean
	inline ������ ���������������������������(����������<������> ��������������������_) { ESB_INVOKE_API_FUNC3(������, addin, AttachAddIn, ��������������������_, ESB_DEFARG(������������), ESB_DEFARG(������������)); };
	// Void LoadAddIn(String FileName_)
	inline void ��������������������������(����������<������> ��������_) { ESB_INVOKE_API_PROC1(addin, LoadAddIn, ��������_); };
	// Void BeginAttachingAddIn(NotifyDescription NotifyDescription_ = <default>, String ObjectId_ = <default>)
	inline void ����������������������������������(����������<������������������> ������������������_ = ��������������������������<������������������>, ����������<������> ��������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC4(addin, BeginAttachingAddIn, ������������������_, ��������������������_, ESB_DEFARG(������������), ESB_DEFARG(������������)); };
	// Void BeginAttachingAddIn(NotifyDescription NotifyDescription_ = <default>, String ObjectId_ = <default>)
	inline void ����������������������������������(����������<������������������> ������������������_, ����������<������> ��������������_, ����������<������> ���_, ����������<��������������������> ���_ = ��������������������������<��������������������>) { ESB_INVOKE_API_PROC4(addin, BeginAttachingAddIn, ������������������_, ��������������_, ���_, ���_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(addin)
#if ESB_API_PROVIDER_INCLUDE(entext0x1)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ������� ����������������(������_ ��� ������, �����������������������_ ��� �����������������������, [���������������_ ��� ������]) ���������� ������
	inline String EncodeString(ConstParam<String> String_, ConstParam<StringEncodingMethod> StringEncodingMethod_, ConstParam<String> StringEncoding_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, entext0x1, EncodeString, String_, StringEncodingMethod_, StringEncoding_); };
	// ������� �������������������(������_ ��� ������, �����������������������_ ��� �����������������������, [���������������_ ��� ������]) ���������� ������
	inline String DecodeString(ConstParam<String> String_, ConstParam<StringEncodingMethod> StringEncodingMethod_, ConstParam<String> StringEncoding_ = DefUndef<String>) { ESB_INVOKE_API_FUNC3(String, entext0x1, DecodeString, String_, StringEncodingMethod_, StringEncoding_); };
	// ������� ����������������������������() ���������� ������
	inline String GetServerAllFilesMask() { ESB_INVOKE_API_FUNC0(String, entext0x1, GetServerAllFilesMask); };
	// ������� ������������������������������() ���������� ������
	inline String GetServerPathSeparator() { ESB_INVOKE_API_FUNC0(String, entext0x1, GetServerPathSeparator); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Function EncodeString(String String_, StringEncodingMethod StringEncodingMethod_, String StringEncoding_ = <default>) -> String
	inline ������ ����������������(����������<������> ������_, ����������<�����������������������> �����������������������_, ����������<������> ���������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC3(������, entext0x1, EncodeString, ������_, �����������������������_, ���������������_); };
	// Function DecodeString(String String_, StringEncodingMethod StringEncodingMethod_, String StringEncoding_ = <default>) -> String
	inline ������ �������������������(����������<������> ������_, ����������<�����������������������> �����������������������_, ����������<������> ���������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC3(������, entext0x1, DecodeString, ������_, �����������������������_, ���������������_); };
	// Function GetServerAllFilesMask() -> String
	inline ������ ����������������������������() { ESB_INVOKE_API_FUNC0(������, entext0x1, GetServerAllFilesMask); };
	// Function GetServerPathSeparator() -> String
	inline ������ ������������������������������() { ESB_INVOKE_API_FUNC0(������, entext0x1, GetServerPathSeparator); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(entext0x1)
#if ESB_API_PROVIDER_INCLUDE(mngui0x7)
#if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
	// ��������� ������������������������������([�����_ ��� ������], [�������������������_ ��� ������], [���������_ ��� ������], [��������_ ��� ��������])
	inline void ShowUserNotification(ConstParam<String> Text_ = DefUndef<String>, ConstParam<String> NavigationLink_ = DefUndef<String>, ConstParam<String> Explanation_ = DefUndef<String>, ConstParam<Picture> Picture_ = DefUndef<Picture>) { ESB_INVOKE_API_PROC4(mngui0x7, ShowUserNotification, Text_, NavigationLink_, Explanation_, Picture_); };
	// ������� ������������(�����_ ��� ������, [��������_ ��� ������], [������������_ ��� ������]) ���������� ������<������, ������������>
	inline UndefOr<Boolean> GetFile(ConstParam<String> Address_, ConstParam<String> FileName_ = DefUndef<String>, ConstParam<Boolean> Interactively_ = DefUndef<Boolean>) { ESB_INVOKE_API_FUNC3(UndefOr<Boolean>, mngui0x7, GetFile, Address_, FileName_, Interactively_); };
	// ������� �������������([�����_ ��� ������], [�����������������_ ��� ������], [�����������������_ ��� ������], [������������_ ��� ������], [����������������������������_ ��� �����������������������]) ���������� ������
	inline Boolean PutFile(ConstParam<String> Address_ = DefUndef<String>, ConstParam<String> FileName_ = DefUndef<String>, ConstParam<String> SelectedFileName_ = DefUndef<String>, ConstParam<Boolean> Interactively_ = DefUndef<Boolean>, ConstParam<UUIDValue> FormUniqueID_ = DefUndef<UUIDValue>) { ESB_INVOKE_API_FUNC5(Boolean, mngui0x7, PutFile, Address_, FileName_, SelectedFileName_, Interactively_, FormUniqueID_); };
	// ������� ��������������������([�����������������������������_ ��� ������������������], [�����_ ��� ������], [�����������������_ ��� ������], [������������_ ��� ������], [����������������������������_ ��� �����������������������]) ���������� ������
	//!> the method is actually a procedure according to disp-information.
	// ������� �������������������������������������(���_ ��� ������) ���������� ������������
	inline Arbitrary GetInterfaceFunctionalOption(ConstParam<String> Name_) { ESB_INVOKE_API_FUNC1(Arbitrary, mngui0x7, GetInterfaceFunctionalOption, Name_); };
	// ��������� ������������������������������������������������(������������������������_ ��� ���������, [���������������������_ ��� ������<������, ������>])
	inline void SetInterfaceFunctionalOptionParameters(ConstParam<Structure> ParametersToBeSpecified_, ConstParam<String> ParametersToBeReset_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(mngui0x7, SetInterfaceFunctionalOptionParameters, ParametersToBeSpecified_, ParametersToBeReset_); };
	inline void SetInterfaceFunctionalOptionParameters(ConstParam<Structure> ParametersToBeSpecified_, ConstParam<Array> ParametersToBeReset_ = DefUndef<Array>) { ESB_INVOKE_API_PROC2(mngui0x7, SetInterfaceFunctionalOptionParameters, ParametersToBeSpecified_, ParametersToBeReset_); };
	// ������� ����������������������������������������������() ���������� ���������
	inline Structure GetInterfaceFunctionalOptionParameters() { ESB_INVOKE_API_FUNC0(Structure, mngui0x7, GetInterfaceFunctionalOptionParameters); };
	// ��������� �����������������()
	inline void RefreshInterface() { ESB_INVOKE_API_PROC0(mngui0x7, RefreshInterface); };
	// ��������� ������������������������������������(����������������_ ��� ������)
	inline void SetShortApplicationCaption(ConstParam<String> ShortCaption_) { ESB_INVOKE_API_PROC1(mngui0x7, SetShortApplicationCaption, ShortCaption_); };
	// ������� ����������������������������������() ���������� ������
	inline String GetShortApplicationCaption() { ESB_INVOKE_API_FUNC0(String, mngui0x7, GetShortApplicationCaption); };
	// ��������� ����������������������������(�������������������_ ��� ������)
	inline void GotoURL(ConstParam<String> NavigationLink_) { ESB_INVOKE_API_PROC1(mngui0x7, GotoURL, NavigationLink_); };
	// ��������� ����������������������������(���������������������_ ��� ������������������, [���������_ ��� ������������])
	inline void ExecuteNotifyProcessing(ConstParam<NotifyDescription> NotificationPerformed_, ConstParam<Arbitrary> Result_ = DefUndef<Arbitrary>) { ESB_INVOKE_API_PROC2(mngui0x7, ExecuteNotifyProcessing, NotificationPerformed_, Result_); };
	// ��������� ��������������(�������������_ ��� ������, [��������������_ ��� ������])
	inline void System(ConstParam<String> CommandString_, ConstParam<String> CurrentDirectory_ = DefUndef<String>) { ESB_INVOKE_API_PROC2(mngui0x7, System, CommandString_, CurrentDirectory_); };
	// ������� ����������������������������������() ���������� ������
	inline Boolean AttachFileSystemExtension() { ESB_INVOKE_API_FUNC0(Boolean, mngui0x7, AttachFileSystemExtension); };
	// ��������� �����������������������������������������(������������������_ ��� ������������������)
	inline void BeginAttachingFileSystemExtension(ConstParam<NotifyDescription> NotifyDescription_) { ESB_INVOKE_API_PROC1(mngui0x7, BeginAttachingFileSystemExtension, NotifyDescription_); };
	// ��������� ����������������������������������()
	inline void InstallFileSystemExtension() { ESB_INVOKE_API_PROC0(mngui0x7, InstallFileSystemExtension); };
	// ��������� ���������������������������������������([�����������������������������_ ��� ������������������])
	inline void BeginInstallFileSystemExtension(ConstParam<NotifyDescription> NotifyDescriptionOnCompletion_ = DefUndef<NotifyDescription>) { ESB_INVOKE_API_PROC1(mngui0x7, BeginInstallFileSystemExtension, NotifyDescriptionOnCompletion_); };
	// ��������� ���������������������������(��������������_ ��� ������)
	inline void InstallAddIn(ConstParam<String> Location_) { ESB_INVOKE_API_PROC1(mngui0x7, InstallAddIn, Location_); };
	// ��������� ��������������������������������([�����������������������������_ ��� ������������������], ��������������_ ��� ������)
	inline void BeginInstallAddIn(ConstParam<UndefOr<NotifyDescription>> NotifyDescriptionOnCompletion_, ConstParam<String> Location_) { ESB_INVOKE_API_PROC2(mngui0x7, BeginInstallAddIn, NotifyDescriptionOnCompletion_, Location_); };
	// ������� �������������������������������(������_ ��� ������) ���������� ������
	inline Boolean RequestUserPermission(ConstParam<Array> Calls_) { ESB_INVOKE_API_FUNC1(Boolean, mngui0x7, RequestUserPermission, Calls_); };
	// ��������� ����������������������������������(������������������_ ��� ������������������, ������_ ��� ������)
	inline void BeginRequestingUserPermission(ConstParam<NotifyDescription> NotifyDescription_, ConstParam<Array> Calls_) { ESB_INVOKE_API_PROC2(mngui0x7, BeginRequestingUserPermission, NotifyDescription_, Calls_); };
	// ������� ��������������������������������������() ���������� ������
	inline String GetClientApplicationCaption() { ESB_INVOKE_API_FUNC0(String, mngui0x7, GetClientApplicationCaption); };
	// ��������� ����������������������������������������(����������������������������������_ ��� ������)
	inline void SetClientApplicationCaption(ConstParam<String> ClientApplicationPresentation_) { ESB_INVOKE_API_PROC1(mngui0x7, SetClientApplicationCaption, ClientApplicationPresentation_); };
#endif	// #if defined(ESB_USE_ENGLISH) && ESB_USE_ENGLISH
#if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
	// Void ShowUserNotification(String Text_ = <default>, String NavigationLink_ = <default>, String Explanation_ = <default>, Picture Picture_ = <default>)
	inline void ������������������������������(����������<������> �����_ = ��������������������������<������>, ����������<������> �������������������_ = ��������������������������<������>, ����������<������> ���������_ = ��������������������������<������>, ����������<��������> ��������_ = ��������������������������<��������>) { ESB_INVOKE_API_PROC4(mngui0x7, ShowUserNotification, �����_, �������������������_, ���������_, ��������_); };
	// Function GetFile(String Address_, String FileName_ = <default>, Boolean Interactively_ = <default>) -> OneOf<Boolean, Undefined>
	inline ���������������<������> ������������(����������<������> �����_, ����������<������> ��������_ = ��������������������������<������>, ����������<������> ������������_ = ��������������������������<������>) { ESB_INVOKE_API_FUNC3(���������������<������>, mngui0x7, GetFile, �����_, ��������_, ������������_); };
	// Function PutFile(String Address_ = <default>, String FileName_ = <default>, String SelectedFileName_ = <default>, Boolean Interactively_ = <default>, UUID FormUniqueID_ = <default>) -> Boolean
	inline ������ �������������(����������<������> �����_ = ��������������������������<������>, ����������<������> �����������������_ = ��������������������������<������>, ����������<������> �����������������_ = ��������������������������<������>, ����������<������> ������������_ = ��������������������������<������>, ����������<�����������������������> ����������������������������_ = ��������������������������<�����������������������>) { ESB_INVOKE_API_FUNC5(������, mngui0x7, PutFile, �����_, �����������������_, �����������������_, ������������_, ����������������������������_); };
	// Function BeginPutFile(NotifyDescription NotifyDescriptionOnCompletion_ = <default>, String Address_ = <default>, String FileName_ = <default>, Boolean Interactively_ = <default>, UUID FormUniqueID_ = <default>) -> Boolean
	//!> the method is actually a procedure according to disp-information.
	// Function GetInterfaceFunctionalOption(String Name_) -> Arbitrary
	inline ������������ �������������������������������������(����������<������> ���_) { ESB_INVOKE_API_FUNC1(������������, mngui0x7, GetInterfaceFunctionalOption, ���_); };
	// Void SetInterfaceFunctionalOptionParameters(Structure ParametersToBeSpecified_, OneOf<Array, String> ParametersToBeReset_ = <default>)
	inline void ������������������������������������������������(����������<���������> ������������������������_, ����������<������> ���������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC2(mngui0x7, SetInterfaceFunctionalOptionParameters, ������������������������_, ���������������������_); };
	inline void ������������������������������������������������(����������<���������> ������������������������_, ����������<������> ���������������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC2(mngui0x7, SetInterfaceFunctionalOptionParameters, ������������������������_, ���������������������_); };
	// Function GetInterfaceFunctionalOptionParameters() -> Structure
	inline ��������� ����������������������������������������������() { ESB_INVOKE_API_FUNC0(���������, mngui0x7, GetInterfaceFunctionalOptionParameters); };
	// Void RefreshInterface()
	inline void �����������������() { ESB_INVOKE_API_PROC0(mngui0x7, RefreshInterface); };
	// Void SetShortApplicationCaption(String ShortCaption_)
	inline void ������������������������������������(����������<������> ����������������_) { ESB_INVOKE_API_PROC1(mngui0x7, SetShortApplicationCaption, ����������������_); };
	// Function GetShortApplicationCaption() -> String
	inline ������ ����������������������������������() { ESB_INVOKE_API_FUNC0(������, mngui0x7, GetShortApplicationCaption); };
	// Void GotoURL(String NavigationLink_)
	inline void ����������������������������(����������<������> �������������������_) { ESB_INVOKE_API_PROC1(mngui0x7, GotoURL, �������������������_); };
	// Void ExecuteNotifyProcessing(NotifyDescription NotificationPerformed_, Arbitrary Result_ = <default>)
	inline void ����������������������������(����������<������������������> ���������������������_, ����������<������������> ���������_ = ��������������������������<������������>) { ESB_INVOKE_API_PROC2(mngui0x7, ExecuteNotifyProcessing, ���������������������_, ���������_); };
	// Void System(String CommandString_, String CurrentDirectory_ = <default>)
	inline void ��������������(����������<������> �������������_, ����������<������> ��������������_ = ��������������������������<������>) { ESB_INVOKE_API_PROC2(mngui0x7, System, �������������_, ��������������_); };
	// Function AttachFileSystemExtension() -> Boolean
	inline ������ ����������������������������������() { ESB_INVOKE_API_FUNC0(������, mngui0x7, AttachFileSystemExtension); };
	// Void BeginAttachingFileSystemExtension(NotifyDescription NotifyDescription_)
	inline void �����������������������������������������(����������<������������������> ������������������_) { ESB_INVOKE_API_PROC1(mngui0x7, BeginAttachingFileSystemExtension, ������������������_); };
	// Void InstallFileSystemExtension()
	inline void ����������������������������������() { ESB_INVOKE_API_PROC0(mngui0x7, InstallFileSystemExtension); };
	// Void BeginInstallFileSystemExtension(NotifyDescription NotifyDescriptionOnCompletion_ = <default>)
	inline void ���������������������������������������(����������<������������������> �����������������������������_ = ��������������������������<������������������>) { ESB_INVOKE_API_PROC1(mngui0x7, BeginInstallFileSystemExtension, �����������������������������_); };
	// Void InstallAddIn(String Location_)
	inline void ���������������������������(����������<������> ��������������_) { ESB_INVOKE_API_PROC1(mngui0x7, InstallAddIn, ��������������_); };
	// Void BeginInstallAddIn(NotifyDescription NotifyDescriptionOnCompletion_ = <default>, String Location_)
	inline void ��������������������������������(����������<���������������<������������������>> �����������������������������_, ����������<������> ��������������_) { ESB_INVOKE_API_PROC2(mngui0x7, BeginInstallAddIn, �����������������������������_, ��������������_); };
	// Function RequestUserPermission(Array Calls_) -> Boolean
	inline ������ �������������������������������(����������<������> ������_) { ESB_INVOKE_API_FUNC1(������, mngui0x7, RequestUserPermission, ������_); };
	// Void BeginRequestingUserPermission(NotifyDescription NotifyDescription_, Array Calls_)
	inline void ����������������������������������(����������<������������������> ������������������_, ����������<������> ������_) { ESB_INVOKE_API_PROC2(mngui0x7, BeginRequestingUserPermission, ������������������_, ������_); };
	// Function GetClientApplicationCaption() -> String
	inline ������ ��������������������������������������() { ESB_INVOKE_API_FUNC0(������, mngui0x7, GetClientApplicationCaption); };
	// Void SetClientApplicationCaption(String ClientApplicationPresentation_)
	inline void ����������������������������������������(����������<������> ����������������������������������_) { ESB_INVOKE_API_PROC1(mngui0x7, SetClientApplicationCaption, ����������������������������������_); };
#endif	// #if defined(ESB_USE_RUSSIAN) && ESB_USE_RUSSIAN
#endif	// #if ESB_API_PROVIDER_INCLUDE(mngui0x7)

	
}	// namespace esb (api part)


#endif	// ESB_ES1_API_H_