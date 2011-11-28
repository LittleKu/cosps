http://topic.csdn.net/u/20110724/20/72a4b1c2-34ab-444d-8294-7d49b2fac20c.html

BOOL CXXXDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
    // TODO: Add your specialized code here and/or call the base class
    if ((((LPNMHDR)lParam)->code == NM_RCLICK))
    {
        CPoint pt, pt2;
        GetCursorPos(&pt);
        pt2 = pt;
        m_list.ScreenToClient(&pt);
        CWnd* pWnd = m_list.ChildWindowFromPoint(pt);
        CHeaderCtrl* pHeader = m_list.GetHeaderCtrl();
        if(pWnd && (pWnd->GetSafeHwnd() == pHeader->GetSafeHwnd()))
        {
            HDHITTESTINFO info = {0};
            info.pt = pt;
            pHeader->SendMessage(HDM_HITTEST, 0, (LPARAM)&info);
            CString str;
            str.Format(_T("Column: %d clicked!"), info.iItem);
            AfxMessageBox(str);
        }
    }
    return CDialog::OnNotify(wParam, lParam, pResult);
}