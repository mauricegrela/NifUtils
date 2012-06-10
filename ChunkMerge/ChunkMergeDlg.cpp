// ChunkMergeDlg.cpp : implementation file
//

#include "..\Common\stdafx.h"
#include "ChunkMerge.h"
#include "ChunkMergeDlg.h"
#include "..\Common\FDFileHelper.h"
#include "..\Common\NifConvertUtility2.h"
#include "..\Common\MaterialDefinitions.h"



extern CString   glPathSkyrim;
extern CString   glPathTemplate;


using namespace NifUtility;

// CChunkMergeDlg dialog




CChunkMergeDlg::CChunkMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChunkMergeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChunkMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChunkMergeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTTON_INPUT, &CChunkMergeDlg::OnBnClickedButtonInput)
  ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &CChunkMergeDlg::OnBnClickedButtonOutput)
  ON_BN_CLICKED(IDOK, &CChunkMergeDlg::OnBnClickedOk)
  ON_BN_CLICKED(IDC_RADIO_COLLISION_1, &CChunkMergeDlg::OnBnClickedRadioCollision)
  ON_BN_CLICKED(IDC_RADIO_COLLISION_2, &CChunkMergeDlg::OnBnClickedRadioCollision)
  ON_BN_CLICKED(IDC_RADIO_COLLISION_3, &CChunkMergeDlg::OnBnClickedRadioCollision)
END_MESSAGE_MAP()


void CChunkMergeDlg::parseDir(CString path, set<string>& directories, bool doDirs)
{
  CFileFind   finder;
  BOOL        result(FALSE);

  result = finder.FindFile(path + _T("\\*.*"));

  while (result)
  {
    result = finder.FindNextFileW();

    if (finder.IsDots())    continue;
    if (finder.IsDirectory() && doDirs)
    {
      CString   newDir(finder.GetFilePath());
      CString   tDir = newDir.Right(newDir.GetLength() - newDir.Find(_T("\\Textures\\")) - 1);

      directories.insert(CStringA(tDir).GetString());

      parseDir(newDir, directories);
    }
    else if (!finder.IsDirectory() && !doDirs)
    {
      CString   newDir(finder.GetFilePath());
      CString   tDir = newDir.Right(newDir.GetLength() - path.GetLength() - 1);

      directories.insert(CStringA(tDir).GetString());
    }
  }
}

// CChunkMergeDlg message handlers

BOOL CChunkMergeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	((CButton*) GetDlgItem(IDC_RADIO_COLLISION_2))->SetCheck(BST_CHECKED);
	((CButton*) GetDlgItem(IDC_RADIO_COLLMAT_1))  ->SetCheck(BST_CHECKED);

	//  temp. until working
	GetDlgItem(IDC_RADIO_COLLMAT_3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_COLLMAT) ->EnableWindow(FALSE);

	//  get templates
	CComboBox*  pCBox = (CComboBox*) GetDlgItem(IDC_COMBO_TEMPLATE);
	CString     pathTemplate(glPathTemplate);
	set<string> directories;

	parseDir(pathTemplate, directories, false);

	for (set<string>::iterator tIter = directories.begin(); tIter != directories.end(); tIter++)
	{
		pCBox->AddString(CString((*tIter).c_str()));
	}
	pCBox->SetCurSel(0);

	//  initialize materials
	pCBox = (CComboBox*) GetDlgItem(IDC_COMBO_COLLMAT);

	for (size_t t(0); t < glMaterialListSize; ++t)
	{
		pCBox->InsertString  (t, CString(glMaterialList[t]._name.c_str()));
		pCBox->SetItemDataPtr(t, (void*) glMaterialList[t]._code);
	}
	pCBox->SelectString(-1, _T("Stone"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChunkMergeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChunkMergeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChunkMergeDlg::OnBnClickedButtonInput()
{
  m_fileNameAry[0] = FDFileHelper::getFileOrFolder(m_fileNameAry[0], L"Nif Files (*.nif)|*.nif||", L"nif");
  GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(m_fileNameAry[0]);
  if (!m_fileNameAry[0].IsEmpty() && !m_fileNameAry[1].IsEmpty())
  {
    GetDlgItem(IDOK)->EnableWindow(TRUE);
  }
}

void CChunkMergeDlg::OnBnClickedButtonOutput()
{
  m_fileNameAry[1] = FDFileHelper::getFileOrFolder(m_fileNameAry[1], L"Nif Files (*.nif)|*.nif|Object Files (*.obj)|*.obj||", L"nif");
  GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText(m_fileNameAry[1]);
  if (!m_fileNameAry[0].IsEmpty() && !m_fileNameAry[1].IsEmpty())
  {
    GetDlgItem(IDOK)->EnableWindow(TRUE);
  }
}

void CChunkMergeDlg::OnBnClickedRadioCollision()
{
}

void CChunkMergeDlg::OnBnClickedOk()
{
  NifConvertUtility2		ncUtility;
  map<int, unsigned int>	materialMap;
  string					infoMessage("Nif successfully converted");
  unsigned short			ncReturn   (NCU_OK);

  //  copy strings from input
  GetDlgItem(IDC_EDIT_INPUT)    ->GetWindowTextW(m_fileNameAry[0]);
  GetDlgItem(IDC_EDIT_OUTPUT)   ->GetWindowTextW(m_fileNameAry[1]);
  GetDlgItem(IDC_COMBO_TEMPLATE)->GetWindowTextW(m_fileNameAry[2]);

  //  get material handling
  MaterialTypeHandling	matHandling((MaterialTypeHandling) (GetCheckedRadioButton(IDC_RADIO_COLLMAT_1, IDC_RADIO_COLLMAT_3) - IDC_RADIO_COLLMAT_1));

  switch (matHandling)
  {
	case NCU_MT_SINGLE:
	{
		CComboBox*	pCBox((CComboBox*) GetDlgItem(IDC_COMBO_COLLMAT));

		materialMap[-1] = ((unsigned int) pCBox->GetItemDataPtr(pCBox->GetCurSel()));
		break;
	}

	case NCU_MT_NITRISHAPE_NAME:
	{
		break;
	}

	case NCU_MT_MATMAP:
	{
		//  take material map from extra dialog
		break;
	}
  }

  //  set flags
  ncUtility.setCollisionNodeHandling((CollisionNodeHandling) (GetCheckedRadioButton(IDC_RADIO_COLLISION_1, IDC_RADIO_COLLISION_2) - IDC_RADIO_COLLISION_1));
  ncUtility.setMaterialTypeHandling (matHandling, materialMap);

  //  convert nif
  ncReturn = ncUtility.addCollision((CStringA(m_fileNameAry[1])).GetString(), (CStringA(m_fileNameAry[0])).GetString(), (CStringA(glPathTemplate + L"\\" + m_fileNameAry[2])).GetString());
  if (ncReturn != NCU_OK)
  {
    infoMessage = "NifConverter returned code: " + ncReturn;
  }

  //  generate info message
  vector<string>  userMessages = ncUtility.getUserMessages();

  infoMessage += "\n\nMessages:\n";
  for (vector<string>::iterator texIter = userMessages.begin(); texIter != userMessages.end(); texIter++)
  {
    infoMessage += ("- " + (*texIter) + "\n");
  }

  MessageBox(CString((const char*) infoMessage.c_str()), L"Convert Info", MB_OK| MB_ICONINFORMATION);
}

