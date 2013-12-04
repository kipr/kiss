#include "system_utils.hpp"

#import <Cocoa/Cocoa.h>

using namespace kiss;

bool SystemUtils::supportsMoveToTrash()
{
  return true;
}

bool SystemUtils::moveToTrash(const QString &path)
{
  NSString *fullPath = [[NSString stringWithCString:path.toUtf8() encoding:NSASCIIStringEncoding] autorelease];
  return [[NSWorkspace sharedWorkspace] performFileOperation:NSWorkspaceRecycleOperation 
                                 source:[fullPath stringByDeletingLastPathComponent]
                                 destination:@""
                                 files:[NSArray arrayWithObject:[fullPath lastPathComponent]]
                                 tag:nil] == YES;
}