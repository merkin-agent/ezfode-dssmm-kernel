/* Reuse the real in-memory FatFs fixture; counters are injected only in its
   temporary host copy of ff.c, never in firmware or on the target. */
#define main existing_long_name_tests
#include "fatfs-long-name-host.c"
#undef main
unsigned long metric_dir_find, metric_dir_next, metric_f_open, metric_f_read;
static void reset_metrics(void)
{
    metric_dir_find=metric_dir_next=metric_f_open=metric_f_read=0;
}
static unsigned long open_case(const char *directory, unsigned n, int missing)
{
    char path[180]; FIL file; FRESULT result; UINT got; char bytes[3];
    snprintf(path,sizeof(path),"%s/Game Collection Item %03u.gba",directory,missing ? n+9 : n-1);
    reset_metrics();
    result=f_open(&file,path,FA_READ);
    if(missing) check(result==FR_NO_FILE,"missing file stays absent");
    else
    {
        check(result==FR_OK,"late-position existing file opens");
        check(f_read(&file,bytes,sizeof(bytes),&got)==FR_OK && got==3 && !memcmp(bytes,"rom",3),"opened identity has expected payload");
        check(f_close(&file)==FR_OK,"close measured file");
    }
    check(metric_f_open==1,"one measured open");
    printf("FatFs N=%u %s: opens=%lu reads=%lu dir_find=%lu dir_next=%lu\n",n,missing?"missing":"last ordinary LFN",metric_f_open,metric_f_read,metric_dir_find,metric_dir_next);
    return metric_dir_next;
}
int main(void)
{
    const unsigned counts[]={30,60,90,120,240};
    unsigned i, entries; unsigned long prev_hit=0,prev_miss=0,hit,miss;
    char directory[24];
    memset(disk,0,sizeof(disk)); format_disk();
    for(i=0;i<sizeof(counts)/sizeof(counts[0]);i++)
    {
        snprintf(directory,sizeof(directory),"0:/SCALE%u",counts[i]);
        entries=run_indexed_name_performance_case(counts[i],directory);
        printf("indexed N=%u: complete names 0 resolver scans; clipped scan=%u returned entries\n",counts[i],entries);
        hit=open_case(directory,counts[i],0); miss=open_case(directory,counts[i],1);
        check(hit>prev_hit && miss>prev_miss,"directory traversal grows with fixture size");
        prev_hit=hit; prev_miss=miss;
    }
    puts("Scaling evidence only: dir_next counts include LFN slots/parents, not SD latency or frame time. No per-move simulation.");
    return 0;
}
