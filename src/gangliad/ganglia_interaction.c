

/*
 * Welcome to the bizarro world of ganglia.h!
 *
 * Ganglia provides a useful library for sending metrics, but it is not
 * compatible with C++
 *
 * Hence, anytime we want to interact with ganglia.h, we must do it in a
 * separate compilation unit written in C.
 */

#include "ganglia.h"

int
ganglia_reconfig(const char *config_file, Ganglia_pool *context, Ganglia_gmond_config *config, Ganglia_udp_send_channels *send_channels)
{
    if (*config)
    {
        // This function is defined in the header but does not actually exist in the library.
        //Ganglia_gmond_config_destroy(*config);
        *config = NULL;
    }
    if (*send_channels)
    {
        // This function is defined in the header but does not actually exist in the library.
        //Ganglia_udp_send_channels_destroy(*send_channels);
        *send_channels = NULL;
    }
    if (*context)
    {
        Ganglia_pool_destroy(*context);
        *context = NULL;
    }

    *context = Ganglia_pool_create(NULL);
    if (!*context)
    {
        return 1;
    }

    char * my_config_file = strdup(config_file);
    if (!my_config_file) return 1;
    *config = Ganglia_gmond_config_create(my_config_file, 0);
    free(my_config_file);
    if (!*config)
    {
        return 1;
    }

    *send_channels = Ganglia_udp_send_channels_create(*context, *config);
    if (!*send_channels)
    {
        return 1;
    }

    return 0;
}


int
ganglia_send(Ganglia_pool context, Ganglia_udp_send_channels channels, const char *group, const char *name, const char *value, const char *type, const char *units, int slope, const char *title, const char *desc, const char *spoof_host, const char *cluster, int tmax, int dmax)
{
    Ganglia_metric metric = Ganglia_metric_create(context);
    if (!metric) return 1;

    char * my_group = strdup(group); if (!my_group) return 4;
    char * my_name  = strdup(name);  if (!my_name)  return 4;
    char * my_value = strdup(value); if (!my_value) return 4;
    char * my_units = strdup(units); if (!my_units) return 4;
    char * my_type  = strdup(type);  if (!my_type)  return 4;
	char * my_title = strdup(title ? title : ""); if (!my_title) return 4;
	char * my_desc = strdup(desc ? desc : "");    if (!my_desc)  return 4;
	char * my_spoof_host = strdup(spoof_host ? spoof_host : ""); if (!my_spoof_host)  return 4;
	char * my_cluster = strdup(cluster ? cluster : ""); if(!my_cluster) return 4;

    int retval = 0;
    if (!Ganglia_metric_set(metric, my_name, my_value, my_type, my_units, slope, tmax, dmax))
    {
        Ganglia_metadata_add(metric, "GROUP", my_group);
		if( *my_title ) {
			Ganglia_metadata_add(metric, "TITLE", my_title);
		}
		if( *my_desc ) {
			Ganglia_metadata_add(metric, "DESC", my_desc);
		}
		if( *my_spoof_host ) {
			Ganglia_metadata_add(metric, "SPOOF_HOST", my_spoof_host);
		}
		if( *my_cluster ) {
			Ganglia_metadata_add(metric, "CLUSTER", my_cluster);
		}
        if (Ganglia_metric_send(metric, channels))
        {
            retval = 2;
        }
    } else
    {
        retval = 3;
    }
    Ganglia_metric_destroy(metric);
    free(my_group);
    free(my_name);
    free(my_value);
    free(my_units);
    free(my_type);
	free(my_title);
	free(my_desc);
	free(my_spoof_host);
	free(my_cluster);
    return retval;
}

