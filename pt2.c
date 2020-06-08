#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/reg.h> 
#include <sys/syscall.h>   /* For SYS_write etc */
#define debug 0

const int long_size = sizeof(long);

int copy_to_fn(char * itr, int child, long addr){
  union u {
    long val;
    char chars[long_size];
  }data;
  int len = 0;
  int i=0;
  int flag =1;
  while(flag){
    data.val = ptrace(PTRACE_PEEKDATA, child, addr + i*4, NULL);
    int j;
    for(j=0;j<long_size;j++){
      if(data.chars[j]=='\0'){
        flag=0;
        break;
      }
    }
    memcpy(itr, data.chars, j);
    itr+=j;
    len+=j;
    if(flag==0){
      (*itr) = '\0';
      break;
    }
    i++;
  }
  return len;
}
void copy_to_ebx(int child, long addr){
  union u {
    long val;
    char chars[long_size];
  }d;
  char * path_to_tmpp = "/tmp/ytn_4439/tmpp";
  for(int i=0;i<20;i=i+4){
    d.chars[0]=path_to_tmpp[i];
    d.chars[1]=path_to_tmpp[i+1];
    d.chars[2]=path_to_tmpp[i+2];
    d.chars[3]=path_to_tmpp[i+3];
    ptrace(PTRACE_POKEDATA, child, addr + i, d.val);
  }
}

int main(int argc,  const char *argv[])
{   pid_t child;
    long orig_eax, eax;
    long params[3];
    int status;
    int insyscall_open = 0;
    int insyscall_stat = 0;
    int insyscall_openat = 0;
    int insyscall_symlink = 0;
    // struct user_regs_struct regs;
    int made_file=0;
    system("rm -rf /tmp/ytn_4439; mkdir /tmp/ytn_4439");
    char * fn = calloc(4097,sizeof(char));
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], argv+1);
    }
    else {
       while(1) {
          wait(&status);
          if(WIFEXITED(status))
              break;
          orig_eax = ptrace(PTRACE_PEEKUSER,
                            child, 4 * ORIG_EAX,
                            NULL);
          if(orig_eax == SYS_open) {
              if(insyscall_open == 0) {
                 /* Syscall entry */
                 insyscall_open = 1;
                 params[0] = ptrace(PTRACE_PEEKUSER,
                                  child, 4 * EBX,
                                  NULL);
                 params[1] = ptrace(PTRACE_PEEKUSER,
                                    child, 4 * ECX,
                                    NULL);
                 params[2] = ptrace(PTRACE_PEEKUSER,
                                    child, 4 * EDX,
                                    NULL);
                 #ifdef DEBUG
                 printf("Open called with %ld, %ld, %ld\n", params[0],params[1],params[2]);
                 #endif
                 int len = copy_to_fn(fn, child, params[0]);
                 
                 if(strncmp(fn,"http://",7)==0 || strncmp(fn,"https://",8)==0){
                    #ifdef DEBUG
                    printf("---------------------------------------OPEN---------------\n");
                    printf("%s %d\n",fn,len);
                    #endif
                    char * perm = "wget -q -O /tmp/ytn_4439/tmpp ";
                    char * sys_arg = malloc(50 + len);
                    int i;
                    for(i=0;i<strlen(perm);i++){
                      sys_arg[i]=perm[i];
                    }
                    for(int j = 0;j<len; j++){
                      sys_arg[i]=fn[j];
                      i++;
                    }
                    sys_arg[i]='\0';
                    #ifdef DEBUG
                    printf("SYSTEM CALL -- %s\n", sys_arg);
                    #endif
                    system(sys_arg);
                    made_file=1;
                    copy_to_ebx(child, params[0]);
                    free(sys_arg);

                 }
                 //ptrace(PTRACE_POKEDATA, child, 4*EBX, (void *)fn);
             }
             else { /* Syscall exit */
                 eax = ptrace(PTRACE_PEEKUSER,
                              child, 4 * EAX,
                              NULL);
                 #ifdef DEBUG
                 printf("Open returned with %ld\n", eax);
                 #endif
                 insyscall_open = 0;
             }
          }




          if(orig_eax == SYS_stat64 || orig_eax== SYS_lstat64) {
              if(insyscall_stat == 0) {
                 /* Syscall entry */
                 insyscall_stat = 1;
                 params[0] = ptrace(PTRACE_PEEKUSER,
                                  child, 4 * EBX,
                                  NULL);
                 params[1] = ptrace(PTRACE_PEEKUSER,
                                    child, 4 * ECX,
                                    NULL);
                 #ifdef DEBUG
                 printf("Stat called with %ld, %ld, %ld\n", params[0],params[1]);
                 #endif
                 int len = copy_to_fn(fn, child, params[0]);
                 
                 if(strncmp(fn,"http://",7)==0 || strncmp(fn,"https://",8)==0){
                    #ifdef DEBUG
                    printf("---------------------------------------STAT64---------------\n");
                    printf("%s %d\n",fn,len);
                    #endif
                    char * perm = "wget -q -O /tmp/ytn_4439/tmpp ";
                    char * sys_arg = malloc(50 + len);
                    int i;
                    for(i=0;i<strlen(perm);i++){
                      sys_arg[i]=perm[i];
                    }
                    for(int j = 0;j<len; j++){
                      sys_arg[i]=fn[j];
                      i++;
                    }
                    sys_arg[i]='\0';
                    #ifdef DEBUG
                    printf("SYSTEM CALL -- %s\n", sys_arg);
                    #endif
                    system(sys_arg);
                    made_file=1;
                    copy_to_ebx(child, params[0]);
                    free(sys_arg);
                    //MAKE FILE READONLY
                    system("chmod 444 /tmp/ytn_4439/tmpp");
                 }
             }
             else { /* Syscall exit */
                 eax = ptrace(PTRACE_PEEKUSER,child, 4 * EAX, NULL);
                 #ifdef DEBUG
                 printf("Stat returned with %ld\n", eax);
                 #endif 
                 insyscall_stat = 0;
             }
         }    










          if(orig_eax == SYS_openat || orig_eax == SYS_linkat) {
              if(insyscall_openat == 0) {
                 /* Syscall entry */
                 insyscall_openat = 1;
                 params[0] = ptrace(PTRACE_PEEKUSER,
                                  child, 4 * EBX,
                                  NULL);
                 params[1] = ptrace(PTRACE_PEEKUSER,
                                    child, 4 * ECX,
                                    NULL);
                 #ifdef DEBUG
                 printf("Openat called with %ld, %ld\n", params[0],params[1]);
                 #endif
                 int len = copy_to_fn(fn, child, params[1]);
                 
                 if(strncmp(fn,"http://",7)==0 || strncmp(fn,"https://",8)==0){
                    #ifdef DEBUG
                    printf("---------------------------------------OPENAT/LINKAT---------------\n");
                    printf("%s %d\n",fn,len);
                    #endif
                    char * perm = "wget -q -O /tmp/ytn_4439/tmpp ";
                    char * sys_arg = malloc(50 + len);
                    int i;
                    for(i=0;i<strlen(perm);i++){
                      sys_arg[i]=perm[i];
                    }
                    for(int j = 0;j<len; j++){
                      sys_arg[i]=fn[j];
                      i++;
                    }
                    sys_arg[i]='\0';
                    #ifdef DEBUG
                    printf("SYSTEM CALL -- %s\n", sys_arg);
                    #endif
                    system(sys_arg);
                    made_file=1;
                    copy_to_ebx(child, params[1]);
                    free(sys_arg);
                    //MAKE FILE READONLY
                    system("chmod 444 /tmp/ytn_4439/tmpp");
                 }
             }
             else { /* Syscall exit */
                 eax = ptrace(PTRACE_PEEKUSER,child, 4 * EAX, NULL);
                 #ifdef DEBUG
                 printf("Openat/Linkat returned with %ld\n", eax);
                 #endif 
                 insyscall_openat = 0;
             }
          }






            if(orig_eax == SYS_symlink) {
              if(insyscall_symlink == 0) {
                 /* Syscall entry */
                 insyscall_symlink = 1;
                 params[0] = ptrace(PTRACE_PEEKUSER,
                                  child, 4 * EBX,
                                  NULL);
                 params[1] = ptrace(PTRACE_PEEKUSER,
                                    child, 4 * ECX,
                                    NULL);
                 params[2] = ptrace(PTRACE_PEEKUSER,
                                    child, 4 * EDX,
                                    NULL);
                 #ifdef DEBUG
                 printf("Symlink called with %ld, %ld, %ld\n", params[0],params[1],params[2]);
                 #endif
                 int len = copy_to_fn(fn, child, params[0]);
                 
                 if(strncmp(fn,"http://",7)==0 || strncmp(fn,"https://",8)==0){
                    #ifdef DEBUG
                    printf("---------------------------------------SYMLINK---------------\n");
                    printf("%s %d\n",fn,len);
                    #endif
                    char * perm = "wget -q -O /tmp/ytn_4439/tmpp ";
                    char * sys_arg = malloc(50 + len);
                    int i;
                    for(i=0;i<strlen(perm);i++){
                      sys_arg[i]=perm[i];
                    }
                    for(int j = 0;j<len; j++){
                      sys_arg[i]=fn[j];
                      i++;
                    }
                    sys_arg[i]='\0';
                    #ifdef DEBUG
                    printf("SYSTEM CALL -- %s\n", sys_arg);
                    #endif
                    system(sys_arg);
                    made_file=1;
                    copy_to_ebx(child, params[0]);
                    free(sys_arg);

                 }
                 //ptrace(PTRACE_POKEDATA, child, 4*EBX, (void *)fn);
             }
             else { /* Syscall exit */
                 eax = ptrace(PTRACE_PEEKUSER,
                              child, 4 * EAX,
                              NULL);
                 #ifdef DEBUG
                 printf("Symlink returned with %ld\n", eax);
                 #endif
                  
                 insyscall_symlink = 0;
             }
          }








          ptrace(PTRACE_SYSCALL, child,
                 NULL, NULL);
       }
   }
   if(made_file==1){
      system("chmod 666 /tmp/ytn_4439/tmpp");
      system("rm /tmp/ytn_4439/tmpp");
   }
   system("rm -rf /tmp/ytn_4439");
   
   return 0;
}