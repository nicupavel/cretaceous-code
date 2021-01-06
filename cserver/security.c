#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>

#include "config.h"

void security_initialize (const server_config_tag * const configuration)
{
   struct passwd *user;
   struct group *group;
   uid_t uid=-1;
   gid_t gid=-1;

   if(configuration->chuid)
   {
       if(configuration->user) {
           user = getpwnam(configuration->user);
           if(user)
               uid = user->pw_uid;
           else
               fprintf(stderr, "Couldn't find user \"%s\" in password file\n", configuration->user);
       }
       if(configuration->group) {
           group = getgrnam(configuration->group);

           if(group)
               gid = group->gr_gid;
           else
               fprintf(stderr, "Couldn't find group \"%s\" in groups file\n", configuration->group);
       }
   }

   if (configuration->chroot)
   {
       if(getuid()) /* root check */
       {
           fprintf(stderr, "WARNING: Cannot change server root unless running as root.\n");
           return;
       }
       if(chroot(configuration->base_dir))
       {
           fprintf(stderr,"WARNING: Couldn't change server root: %s\n", strerror(errno));
           return;
       }
       else
           fprintf(stdout, "Changed root successfully to \"%s\".\n", configuration->base_dir);

   }   

   if(configuration->chuid)
   {
       if(getuid()) /* root check */
       {
           fprintf(stderr, "WARNING: Can't change user id unless you are root.\n");
           return;
       }

       if(gid != -1) {
           if(!setgid(gid))
               fprintf(stdout, "Changed groupid to %i.\n", gid);
           else
               fprintf(stdout, "Error changing groupid: %s.\n", strerror(errno));
       }

       if(uid != -1) {
           if(!setuid(uid))
               fprintf(stdout, "Changed userid to %i.\n", uid);
           else
               fprintf(stdout, "Error changing userid: %s.\n", strerror(errno));
       }
   }
}
