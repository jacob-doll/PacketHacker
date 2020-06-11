#include "byte_viewer.h"

namespace PacketHacker {
namespace UI {

  ByteEditor::ByteEditor() {}
  void ByteEditor::Create(wxWindow *parent,
    wxWindowID id,
    wxEvtHandler *evtHandler)
  {
    wxGridCellTextEditor::Create(parent, id, evtHandler);
    wxTextCtrl *txtCtrl = (wxTextCtrl *)m_control;
    txtCtrl->SetMaxLength(2);
  }

  void ByteEditor::BeginEdit(int row, int col, wxGrid *grid)
  {
    wxGridTableBase *table = grid->GetTable();
    m_Value = table->GetValueAsLong(row, col);
    DoBeginEdit(GetString());
  }

  bool ByteEditor::EndEdit(int row, int col, const wxGrid *grid, const wxString &oldval, wxString *newval)
  {
    long value = 0;
    wxString text = Text()->GetValue();
    if (text.empty()) {
      if (oldval.empty())
        return false;
    } else// non-empty text now (maybe 0)
    {
      if (!text.ToLong(&value, 16))
        return false;

      if (value == m_Value && (value || !oldval.empty()))
        return false;
    }

    m_Value = value;

    if (newval)
      *newval = text;

    return true;
  }

  void ByteEditor::ApplyEdit(int row, int col, wxGrid *grid)
  {
    wxGridTableBase *const table = grid->GetTable();
    table->SetValueAsLong(row, col, m_Value);
  }

  void ByteEditor::Reset()
  {
    DoReset(GetString());
  }

  bool ByteEditor::IsAcceptedKey(wxKeyEvent &event)
  {
    if (wxGridCellEditor::IsAcceptedKey(event)) {
      int keycode = event.GetKeyCode();
      if ((keycode < 128) && (wxIsdigit(keycode) || keycode == 'a' || keycode == 'b' || keycode == 'c' || keycode == 'd' || keycode == 'e' || keycode == 'f')) {
        return true;
      }
    }

    return false;
  }

  void ByteEditor::StartingKey(wxKeyEvent &event)
  {
    int keycode = event.GetKeyCode();

    if (wxIsdigit(keycode) || keycode == 'a' || keycode == 'b' || keycode == 'c' || keycode == 'd' || keycode == 'e' || keycode == 'f') {
      wxGridCellTextEditor::StartingKey(event);
      return;
    }

    event.Skip();
  }

  wxString ByteEditor::GetValue() const
  {
    return Text()->GetValue();
  }

  void ByteEditor::HandleReturn(wxKeyEvent &event)
  {
  }

  ByteTable::ByteTable(int intialSize)
    : wxGridTableBase()
  {
    m_Data.resize(intialSize);
  }

  wxString ByteTable::GetValue(int row, int col)
  {
    if (col >= GetNumberCols() - 1)
      return "";
    int index = col + row * 16;
    if (index >= m_Data.size())
      return ".";
    uint8_t data = (uint8_t)GetValueAsLong(row, col);
    char buffer[4];
    itoa(data, buffer, 16);
    return wxString::Format("%02s", buffer);
  }

  void ByteTable::SetValue(int row, int col, const wxString &value)
  {
    if (col >= GetNumberCols() - 1)
      return;
    long val = 0;
    if (value.ToLong(&val, 16)) {
      SetValueAsLong(row, col, val);
    }
  }

  long ByteTable::GetValueAsLong(int row, int col)
  {
    if (col >= GetNumberCols() - 1)
      return 0xfff;
    int index = col + row * 16;
    if (index >= m_Data.size())
      return 0xfff;
    uint8_t val = m_Data[index];
    return val;
  }

  void ByteTable::SetValueAsLong(int row, int col, long value)
  {
    if (col >= GetNumberCols() - 1)
      return;
    int index = col + row * 16;
    if (index >= m_Data.size()) {
      m_Data.emplace_back((uint8_t)value);
      return;
    }
    m_Data[index] = (uint8_t)value;
  }

  wxString ByteTable::GetRowLabelValue(int row)
  {
    char buffer[4];
    itoa(row, buffer, 16);
    return wxString::Format("%04s", buffer);
  }

  void ByteTable::Clear()
  {
    m_Data.clear();
  }

  bool ByteTable::InsertRows(size_t pos, size_t numRows) { return true; }

  bool ByteTable::AppendRows(size_t numRows)
  {
    m_rows += numRows;

    if (GetView()) {
      wxGridTableMessage msg(this,
        wxGRIDTABLE_NOTIFY_ROWS_APPENDED,
        numRows);

      GetView()->ProcessTableMessage(msg);
    }

    return true;
  }

  bool ByteTable::DeleteRows(size_t pos, size_t numRows) { return false; }

  void ByteTable::Resize(size_t size)
  {
    m_Data.resize(size);
  }

  ByteViewer::ByteViewer(wxWindow *parent,
    wxWindowID winid,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name)
    : wxPanel(parent, wxID_ANY, pos, size, style, name)
  {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    m_pGrid = new wxGrid(this, wxID_ANY);
    m_pByteTable = new ByteTable(8);
    m_pGrid->SetTable(m_pByteTable, true);
    m_pGrid->SetTabBehaviour(wxGrid::Tab_Wrap);
    m_pGrid->HideColLabels();
    m_pGrid->EnableGridLines(false);
    m_pGrid->SetCellHighlightPenWidth(1);
    m_pGrid->SetDefaultColSize(18, true);
    m_pGrid->SetRowLabelSize(40);
    m_pGrid->SetDefaultEditor(new ByteEditor());
    m_pGrid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_BOTTOM);
    for (int i = 0; i < 16; i++) {
      m_pGrid->DisableColResize(i);
    }
    m_pGrid->SetColSize(16, 40);
    sizer->Add(m_pGrid, 1, wxEXPAND);

    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(ByteViewer::OnGridKeyDown));

    this->SetSizerAndFit(sizer);
  }

  void ByteViewer::OnGridKeyDown(wxKeyEvent &event)
  {
    if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER) {
      if (m_pGrid->GetGridCursorRow() == m_pGrid->GetNumberRows() - 1) {
        m_pGrid->AppendRows();
      }
    }
    event.Skip();
  }

  void OnGridCellChanged(wxGridEvent &event)
  {
  }

  std::vector<uint8_t> ByteViewer::GetData()
  {
    return m_pByteTable->GetData();
  }

  void ByteViewer::SetByte(uint32_t offset, uint8_t byte)
  {
    int row = offset / 16;
    int col = offset - row * 16;
    if (row >= m_pGrid->GetNumberRows())
      m_pGrid->AppendRows();
    m_pGrid->GetTable()->SetValueAsLong(row, col, byte);
  }

  void ByteViewer::WriteInt(uint32_t offset, uint64_t val, uint32_t size)
  {
    for (int i = 0; i < size; i++) {
      SetByte(offset + (size - 1 - i), val >> (i * 8));
    }
  }

  void ByteViewer::Update(Packet *packet)
  {
    m_pGrid->ClearGrid();
    uint32_t size = packet->Size();
    uint8_t *data = new uint8_t[size];
    packet->WriteToBuf(data, size);
    for (uint32_t i = 0; i < size; i++) {
      SetByte(i, data[i]);
    }
    m_pGrid->ForceRefresh();

    delete[] data;
  }

  void ByteViewer::SetSize(uint32_t size)
  {
    if (m_pByteTable->GetData().size() < size) {
      int numRows = size / 16;
      while (numRows > 0) {
        m_pGrid->AppendRows();
        numRows -= 1;
      }
      m_pByteTable->Resize(size);
    }
  }

  void ByteViewer::Refresh()
  {
    m_pGrid->ForceRefresh();
  }

}// namespace UI
}// namespace PacketHacker