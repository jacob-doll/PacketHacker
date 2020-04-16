#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <vector>
#include <stdint.h>

namespace PacketHacker
{
namespace UI
{

class ByteEditor : public wxGridCellTextEditor
{
public:
    ByteEditor();
    virtual void Create(wxWindow *parent,
                        wxWindowID id,
                        wxEvtHandler *evtHandler) override;
    virtual void Reset() override;

    virtual bool IsAcceptedKey(wxKeyEvent &event) override;
    virtual void StartingKey(wxKeyEvent &event) override;
    virtual void BeginEdit(int row, int col, wxGrid *grid) override;
    virtual bool EndEdit(int row, int col, const wxGrid *grid,
                         const wxString &oldval, wxString *newval) override;
    virtual void ApplyEdit(int row, int col, wxGrid *grid) override;
    virtual void HandleReturn(wxKeyEvent &event) override;

    virtual wxString GetValue() const override;
    virtual wxGridCellEditor *Clone() const override { return new ByteEditor(); }

    wxString GetString() const
    {
        char buffer[4];
        itoa(m_Value, buffer, 16);
        return wxString::Format(wxT("%02s"), buffer);
    }

private:
    long m_Value = 0;
};

class ByteTable : public wxGridTableBase
{
public:
    ByteTable(int intialSize);

    virtual int GetNumberRows() override { return m_rows; }
    virtual int GetNumberCols() override { return m_cols; }
    virtual wxString GetValue(int row, int col) override;
    virtual void SetValue(int row, int col, const wxString &s) override;

    wxString GetRowLabelValue(int row) override;

    virtual long GetValueAsLong(int row, int col) override;
    virtual void SetValueAsLong(int row, int col, long value) override;

    void Clear() override;
    bool InsertRows(size_t pos = 0, size_t numRows = 1) override;
    bool AppendRows(size_t numRows = 1) override;
    bool DeleteRows(size_t pos = 0, size_t numRows = 1) override;

    std::vector<uint8_t> GetData() { return m_Data; }

private:
    int m_rows = 1;
    const int m_cols = 17;
    std::vector<uint8_t> m_Data;
};

class ByteViewer : public wxPanel
{
public:
    ByteViewer(wxWindow *parent,
               wxWindowID winid = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL | wxNO_BORDER,
               const wxString &name = wxPanelNameStr);

    void SetByte(int offset, uint8_t byte);

    std::vector<uint8_t> GetData();

private:
    void OnGridKeyDown(wxKeyEvent &event);
    void OnGridCellChanged(wxGridEvent &event);

private:
    wxGrid *m_pGrid = nullptr;
    ByteTable *m_pByteTable = nullptr;
};

} // namespace UI

} // namespace PacketHacker