#ifndef __MUTEXS_H__
#define	__MUTEXS_H__

extern pthread_rwlock_t cam_mutex;
extern pthread_rwlock_t zgb_mutex;
extern pthread_rwlock_t gsm_mutex;
extern pthread_rwlock_t zgbenv_mutex;		//zgb采集的环境信息读写锁
extern pthread_rwlock_t gsmcmd_mutex;		//gsm采集的指令读写锁
extern pthread_rwlock_t cam_jpeg_mutex;		//camare rgb的缓存读写锁

extern pthread_mutex_t sync_mutex;
extern pthread_mutex_t M0ctr_mutex;

extern int sync_flag;

#endif
