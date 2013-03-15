#ifndef CPROPERTY_GRID_H
# define CPROPERTY_GRID_H

class CPropertyGrid
{
public:
  CPropertyGrid(CMFCPropertyGridCtrl* propGrid);

private:
  CMFCPropertyGridCtrl*                           m_propGrid;
};

#endif // !CPROPERTY_GRID_H