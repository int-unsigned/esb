#pragma once

// �� �� ���� �� �������. ������ ������


//�������� ���������� ������ ��������� ��� ���������
#define ESB_WARN_NO_OPERATOR_ASSIGN_COPY	4626
//�������� ���������� ������������ ������ ��������� ��� ���������
#define ESB_WARN_NO_OPERATOR_ASSIGN_MOVE	5027
//�������� ���������� ������ ��������� ��� ���������
//�������� ���������� ������������ ������ ��������� ��� ���������
#define ESB_WARN_NO_OPERATOR_ASSIGN_ANY		ESB_WARN_NO_OPERATOR_ASSIGN_COPY  ESB_WARN_NO_OPERATOR_ASSIGN_MOVE
//����� �������� ����������� �������, �� ��� ����������� ���������� �� �������� �����������; ���������� ��� ����������� ����������� �� ����� ������ �������� ����� �������� �����������.
#define ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR	5204
//����� �������� ����������� �������, �� ��� ������������� ���������� �� �������� �����������; ���������� ����������� ����� ������ ����� �������� �����������.
#define ESB_WARN_NO_VIRTUAL_NONTRIVIAL_DTOR	4265
#define ESB_WARN_NO_VIRTUAL_ANY_DTOR		ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR  ESB_WARN_NO_VIRTUAL_NONTRIVIAL_DTOR
//����������� �� ��������� ������ ��������� ��� ���������
#define ESB_WARN_NO_DEFAULT_CTOR			4623
//����������� ����� ������ ��������� ��� ���������
#define ESB_WARN_NO_CTOR_COPY				4625
//����������� ����������� ������ ��������� ��� ���������
#define ESB_WARN_NO_CTOR_MOVE				5026
//esb::DispInfoStatMeth: "3"-�������� ���� ��������� ����� ������-���� "esb::DispInfoStatMeth::has_retval_"
#define ESB_WARN_PADDING					4820
//������������� ���� ������ ���������� ������ �������� ".." (��� esbhlp ��� � ������ ����)
#ifndef ESB_WARN_INCLUDE_RELATIVE
#define ESB_WARN_INCLUDE_RELATIVE			4464
#endif // !ESB_WARN_INCLUDE_RELATIVE
//this: ����������� � ������ ������������� �������� ������
#define ESB_WARN_THIS_FOR_INIT_BASE			4355
//5246: "std::array<DispInfoT,4>::_Elems": ������������� ���������� ������ ���� ��������� � �������� ������
//����: ��� ��������� ������� �� ������ � ������ 16.... � 17.2 ����� ����������. ����� ���������� ������.
//https://stackoverflow.com/questions/70127146/warning-c5246-the-initialization-of-a-subobject-should-be-wrapped-in-braces
#define ESB_WARN_INIT_WITH_BRACES			5246
// cl ����������� ���� �������: warning C4384: ��������� #pragma "detect_mismatch" ������� ������������ ������ � ���������� �������
// �� "Under Investigation" ���� �����. �����.
// https://developercommunity.visualstudio.com/t/incorrect-warning-using-pramga-detect-mismatch-aft/1320137
#define ESB_WARN_DETECT_MISMATCH_IN_SCOPE	4384
// warning C5105: ��������� ���������� �������, ���������� "defined", �� ����������
// ��� "����������������" ������������ � �� � C:\Program Files\Windows Kits\10\Include\10.0.19041.0\um\winbase.h(9531,5)
// ������� ����������� � ������ "������������" ������������� (������ ��� MS-������������� ������������� ��� ���������)
// �� ���������� ����� �� ������. ����� ������
#define ESB_WARN_DEFINED_IN_MACRO			5105


#define ESB_WARNING_SUPRESS( WARN_ )		\
	__pragma(warning(push))					\
	__pragma(warning( disable : WARN_ ))	

#define ESB_WARNING_RESTORE()				\
	__pragma(warning(pop))

#define ESB_WARNING_SUPRESS_NO_VIRTUAL_DTOR_ANY()						\
	ESB_WARNING_SUPRESS( ESB_WARN_NO_VIRTUAL_TRIVIAL_DTOR ESB_WARN_NO_VIRTUAL_NONTRIVIAL_DTOR)

#define ESB_WARNING_SUPRESS_IMPLICIT_ASSIGN_DELETED_WHEN_REF_MEMBER()	\
	ESB_WARNING_SUPRESS( ESB_WARN_NO_OPERATOR_ASSIGN_ANY )



#define ESB_PRAGMA_DETECT_MISMATCH(TAG_, VAL_)	\
	__pragma(detect_mismatch(TAG_, VAL_))


// ������� ��� ����������� � ������� ������
#define ESB_PRAGMA_LINKER(LINKER_OPTION_STRING_)		\
	__pragma( comment(linker, LINKER_OPTION_STRING_) )
