#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "config.h"

/* private vars */
static server_config_tag *configuration;
static char *config_filename;

/* private functions */
static void config_set_defaults (server_config_tag *configuration);
static void config_parse_root(server_config_tag *configuration, xmlDocPtr doc, xmlNodePtr node);
static void config_parse_limits(server_config_tag *configuration, xmlDocPtr doc, xmlNodePtr node);
static void config_parse_security(server_config_tag *configuration,xmlDocPtr doc, xmlNodePtr node);
static void config_parse_paths(server_config_tag *configuration,xmlDocPtr doc, xmlNodePtr node);
static void config_parse_logging(server_config_tag *configuration,xmlDocPtr doc, xmlNodePtr node);

void config_initialize(void)
{
    configuration = (server_config_tag *)malloc(sizeof(server_config_tag));
    memset(configuration, 0, sizeof(server_config_tag));
    config_set_defaults(configuration);
    config_filename = NULL;
}

/* this function can only be called here */
static void config_set_defaults (server_config_tag *configuration)
{
    configuration->admin = (char *) strdup (CONFIG_DEFAULT_ADMIN);
    configuration->client_limit = CONFIG_DEFAULT_CLIENT_LIMIT;
    configuration->client_timeout = CONFIG_DEFAULT_CLIENT_TIMEOUT;
    configuration->hostname = (char *)strdup(CONFIG_DEFAULT_HOSTNAME);
    configuration->port = CONFIG_DEFAULT_PORT;
    configuration->bind_address = NULL;
    configuration->base_dir = (char *)strdup(CONFIG_DEFAULT_BASE_DIR);
    configuration->log_dir = (char *)strdup(CONFIG_DEFAULT_LOG_DIR);
    configuration->access_log = (char *)strdup(CONFIG_DEFAULT_ACCESS_LOG);
    configuration->error_log = (char *)strdup(CONFIG_DEFAULT_ERROR_LOG);
    configuration->chroot = CONFIG_DEFAULT_CHROOT;
    configuration->chuid = CONFIG_DEFAULT_CHUID;
    configuration->user = CONFIG_DEFAULT_USER;
    configuration->group = CONFIG_DEFAULT_GROUP;
}

/* write better returns here */
int config_parse_file(const char *filename)
{
	xmlDocPtr doc;
	xmlNodePtr node;

	if (filename == NULL || strcmp(filename, "") == 0) return 1;
	
	config_filename = (char *)strdup(filename);

	doc = xmlParseFile(config_filename);
	if (doc == NULL) {
		return 1;
	}

	node = xmlDocGetRootElement(doc);
	if (node == NULL) {
		xmlFreeDoc(doc);
		return 1;
	}

	if (strcmp(node->name, "CServer") != 0) {
		xmlFreeDoc(doc);
		return 1;
	}

	config_parse_root(configuration, doc, node->xmlChildrenNode);

	xmlFreeDoc(doc);

	return 0;
}

static void config_parse_root(server_config_tag *configuration,xmlDocPtr doc, xmlNodePtr node)
{
	char *tmp;

	do {
		if (node == NULL) break;
		if (xmlIsBlankNode(node)) continue;

		if (strcmp(node->name, "admin") == 0) {
			if (configuration->admin) free(configuration->admin);
			configuration->admin = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		} else if (strcmp(node->name, "hostname") == 0) {
			if (configuration->hostname) free(configuration->hostname);
			configuration->hostname = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		} else if (strcmp(node->name, "port") == 0) {
			tmp = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
			configuration->port = atoi(tmp);
			if (tmp) free(tmp);
		} else if (strcmp(node->name, "bind-address") == 0) {
			if (configuration->bind_address) free(configuration->bind_address);
			configuration->bind_address = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		} else if (strcmp(node->name, "limits") == 0) {
			config_parse_limits(configuration,doc, node->xmlChildrenNode);
		} else if (strcmp(node->name, "security") == 0) {
			config_parse_security(configuration,doc, node->xmlChildrenNode);
		} else if (strcmp(node->name, "paths") == 0) {
			config_parse_paths(configuration,doc, node->xmlChildrenNode);
		} else if (strcmp(node->name, "logging") == 0) {
			config_parse_logging(configuration,doc, node->xmlChildrenNode);
		}
	} while ((node = node->next));
}

static void config_parse_limits(server_config_tag *configuration,xmlDocPtr doc, xmlNodePtr node)
{
    char *tmp;
	do {
		if (node == NULL) break;
		if (xmlIsBlankNode(node)) continue;

		if (strcmp(node->name, "client-timeout") == 0) {
			tmp = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
			configuration->client_timeout = atoi(tmp);
			if (tmp) free(tmp);
		} else if (strcmp(node->name, "client-limit") == 0) {
			tmp = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
			configuration->client_limit = atoi(tmp);
			if (tmp) free(tmp);
		}
	} while ((node = node->next));
}

static void config_parse_security(server_config_tag *configuration,xmlDocPtr doc, xmlNodePtr node)
{
    char *tmp;
	do {
		if (node == NULL) break;
		if (xmlIsBlankNode(node)) continue;

		if (strcmp(node->name, "chroot") == 0) {
			tmp = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
			configuration->chroot = atoi(tmp);
			if (tmp) free(tmp);
		} else if (strcmp(node->name, "chuid") == 0) {
			tmp = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
			configuration->chuid = atoi(tmp);
			if (tmp) free(tmp);
		}
		else if (strcmp(node->name, "user") == 0) {
			if (configuration->user) free(configuration->user);
			configuration->user = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		}
		else if (strcmp(node->name, "group") == 0) {
			if (configuration->group) free(configuration->group);
			configuration->group = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		}
	} while ((node = node->next));
}

static void config_parse_paths(server_config_tag *configuration,xmlDocPtr doc, xmlNodePtr node)
{
	do {
		if (node == NULL) break;
		if (xmlIsBlankNode(node)) continue;

		if (strcmp(node->name, "base-dir") == 0) {
			if (configuration->base_dir) free(configuration->base_dir);
			configuration->base_dir = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		}
		else if (strcmp(node->name, "log-dir") == 0) {
			if (configuration->log_dir) free(configuration->log_dir);
			configuration->log_dir = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		}
	} while ((node = node->next));
}

static void config_parse_logging(server_config_tag *configuration,xmlDocPtr doc, xmlNodePtr node)
{
	do {
		if (node == NULL) break;
		if (xmlIsBlankNode(node)) continue;

		if (strcmp(node->name, "access-log") == 0) {
			if (configuration->access_log) free(configuration->access_log);
			configuration->access_log = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		}
		else if (strcmp(node->name, "error-log") == 0) {
			if (configuration->error_log) free(configuration->error_log);
			configuration->error_log = (char *)xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
		}
	} while ((node = node->next));
}


void config_dump(server_config_tag *configuration)
{
	printf("admin = %s\n", configuration->admin);
	printf("hostname = %s\n", configuration->hostname);
	printf("port = %d\n", configuration->port);
	printf("bind_address = %s\n", configuration->bind_address);
	printf("limits:\n");
	printf("client_limit = %d\n", configuration->client_limit);
	printf("client_timeout = %d\n", configuration->client_timeout);
	printf("security:\n");
	printf("chroot = %s\n", configuration->chroot ? "yes":"no");
	printf("chuid = %s\n", configuration->chuid ? "yes":"no");
	printf("user = %s\n", configuration->user);
	printf("group = %s\n", configuration->group);
	printf("paths:\n");
	printf("base dir = %s\n", configuration->base_dir);
	printf("log dir = %s\n", configuration->log_dir);
	printf("logging:\n");
	printf("access log = %s\n", configuration->access_log);
	printf("error log = %s\n", configuration->error_log);
	
}

server_config_tag *config_get_config(void)
{
    return configuration;
}
