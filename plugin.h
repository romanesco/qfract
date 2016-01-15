/*
 * $Id: plugin.h,v 1.5 2002/07/17 07:29:59 inou Exp $
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include "config.h"
#include "double.h"

namespace QFract {

class PluginInfo
{
public:
  PluginInfo(const char *n,
	     double xl, double yt, double xr, double yb,
	     int mi, int mo, Parameter p, const char* c,
	     const char* cm=DEFAULT_COLORMAP, const char **pd=NULL)
  { name=n;  xleft=xl; ytop=yt; xright=xr; ybottom=yb;
  maxiter=mi; maxorbit=mo; param.set(p); childname=c;
  colormap=cm; paramdesc=pd;}

  PluginInfo(const char *n,
	     double xl, double yt, double xr, double yb,
	     int mi, int mo, Parameter p, const char* c,
	     const char **pd)
  { name=n;  xleft=xl; ytop=yt; xright=xr; ybottom=yb;
  maxiter=mi; maxorbit=mo; param.set(p); childname=c;
  colormap=DEFAULT_COLORMAP; paramdesc=pd;}

  PluginInfo( const PluginInfo& i )
  { name=i.name;
  xleft=i.xleft; ytop=i.ytop; xright=i.xright; ybottom=i.ybottom;
  maxiter=i.maxiter; maxorbit=i.maxorbit; param.set(i.param);
  childname=i.childname; colormap=i.colormap; paramdesc=i.paramdesc;}
  
  const char *name;
  
  double xleft, ytop, xright, ybottom;
  int maxiter, maxorbit;
  Parameter param;
  
  const char *childname; // the filename of child. NULL if it doesn't exist.
  const char *colormap;
  const char **paramdesc; // descriptions for parameters
};

class Plugin
{
public:
  Plugin(const char *filename);
  ~Plugin();
  Plugin(const Plugin &p);
  void* open(const char *filename);
  int close();
  
  const char *Name() const { return info->name; }
  Point (*Map())( Point, Point, Parameter ) { return map; }
  Point (*Init())( Point, Parameter ) { return init; }
  int (*Iter())( Point, Parameter, int ) { return iter; }
  void (*paramInit())( Parameter ) { return paraminit; }
  
  double XLeft() const { return info->xleft; }
  double YTop() const { return info->ytop; }
  double XRight() const { return info->xright; }
  double YBottom() const { return info->ybottom; }
  Point TopLeft() const { return Point(info->xleft,info->ytop); }
  Point RightBottom() const { return Point(info->xright,info->ybottom); }
  void *Handle() const { return handle; }
  
  const char *ChildName() const { return info->childname; }
  const char *ColorMap() const { return info->colormap; }
  
  int MaxIter() const { return info->maxiter; }
  int MaxOrbit() const { return info->maxorbit; }
  Parameter Param() const { return info->param; }
  const char **ParamDesc() const { return info->paramdesc; }
  
private:
  void *handle;
  PluginInfo* info;
  Point (*map)( Point, Point, Parameter ); // map once
  Point (*init)( Point, Parameter ); // get initial point
  int (*iter)( Point, Parameter, int ); // iteration at most (int) times
  void (*paraminit)( Parameter ); // initialize (static) parameters
                                  // called each time parameters are changed.
};
}
#endif //PLUGIN_H
