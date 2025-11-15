
#include "preferences.h"
#include <string.h>

void preferences_init(Preferences *p,
                      const char *prefID,
                      const char *memberID)
{
    if (!p) return;
    strncpy(p->preferenceID, prefID, PREF_ID_MAX);
    p->preferenceID[PREF_ID_MAX - 1] = '\0';

    strncpy(p->memberID, memberID, PREF_MEMBER_MAX);
    p->memberID[PREF_MEMBER_MAX - 1] = '\0';

    p->suggestion[0] = '\0';
}

void preferences_generateRecommendations(Preferences *p,
                                          const char *memberID)
{
    /* placeholder: in real system you would query history */
    if (!p) return;
    (void)memberID;
    strncpy(p->suggestion, "Try newly arrived books.", PREF_SUGGESTION_MAX);
    p->suggestion[PREF_SUGGESTION_MAX - 1] = '\0';
}

void preferences_recordFeedback(Preferences *p,
                                const char *memberID,
                                const char *feedback)
{
    /* could log per member, here we just overwrite suggestion */
    if (!p) return;
    (void)memberID;
    strncpy(p->suggestion, feedback, PREF_SUGGESTION_MAX);
    p->suggestion[PREF_SUGGESTION_MAX - 1] = '\0';
}
