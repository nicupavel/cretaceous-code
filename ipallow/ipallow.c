
#include <linux/config.h>
#include <linux/module.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netdevice.h>
#include <linux/route.h>
#include <net/route.h>
#include <syslog.h>

#include "ipallow.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nicu Pavel <npavel@ituner.com>");
MODULE_DESCRIPTION("ipallow");
MODULE_PARM(debug, "i");
MODULE_PARM_DESC(debug, "turns module debug off/on (0/1)");
EXPORT_NO_SYMBOLS;

static inline int do_ip_send(struct sk_buff *skb)
{
       return ip_send(skb);
}

static void ip_rewrite(struct sk_buff *skb)
{
	struct iphdr *iph = skb->nh.iph;
	u32 odaddr = iph->saddr;
	u32 osaddr = iph->daddr;

	skb->nfcache |= NFC_ALTERED;

	/* Rewrite IP header */
	iph->daddr = odaddr;
	iph->saddr = osaddr;
}



static unsigned int
ipallow_hook(unsigned int hook, struct sk_buff **pskb,
             const struct net_device *indev, const
	     struct net_device *outdev, int 
	     (*okfn)(struct sk_buff *))
{
					     
/* Get a handle to the packet data 
    unsigned char *data = (void *)(*pskb)->nh.iph +
			    (*pskb)->nh.iph->ihl*4;
*/						       
    (*pskb)->nfcache |= NFC_UNKNOWN;
		  
    switch ((*pskb)->len) {
	case 100:
	    printk("ipallow: corrupting packet\n");
	    if ((*pskb)->dst != NULL) {
			printk("ipallow: packet has destination.\n");
			//ip_rewrite(*pskb);
			//return NF_HOOK(PF_INET, NF_IP_LOCAL_OUT, pskb, NULL, NULL,
			//               do_ip_send);
			return NF_STOLEN;
	    }
	    return NF_DROP;
								
	case 200:
	    printk("ipallow: dropping packet\n");
	    return NF_DROP;
		       
	default:
	    return NF_ACCEPT;
    }
}


static struct nf_hook_ops ipallow_ops
= { { NULL, NULL }, ipallow_hook,
    PF_INET, NF_IP_LOCAL_OUT,
    NF_IP_PRI_FILTER-1 
   };
																	   

static int __init ipallow_init_module(void)
{
    DEBUG(printk (KERN_INFO "ipallow: begin loading.\n"));
    openlog ("ipallow",LOG_PID,LOG_LOCAL0);
    return nf_register_hook(&ipallow_ops);
}


static void __exit ipallow_cleanup_module(void)
{
    DEBUG(printk (KERN_INFO "ipallow: begin unload.\n"));    
    nf_unregister_hook(&ipallow_ops);
}

module_init(ipallow_init_module);
module_exit(ipallow_cleanup_module);

