#pragma once

#include "Common/Compiler.h"

namespace Luna
{
  /////////////////////////////////////////////////////////////////////////////
  // Class for rendering a bitmap within a grid cell.  The name of the bitmap
  // to draw should be stored as a string in the cell.
  // 
  class GridCellImageRenderer : public wxGridCellRenderer
  {
  public:
    GridCellImageRenderer();
    virtual ~GridCellImageRenderer();

  public:
    virtual void Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected ) NOC_OVERRIDE;
    virtual wxSize GetBestSize(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, int row, int col ) NOC_OVERRIDE;
    virtual wxGridCellRenderer* Clone() const NOC_OVERRIDE;
  };
}