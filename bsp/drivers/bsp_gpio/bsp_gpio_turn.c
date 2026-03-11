#include <linux/gpio.h>
#include <linux/gpio/consumer.h> 
#include <linux/module.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/timer.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <stdbool.h>
#include "bsp_gpio_turn.h"

typedef enum{
    NONE_TURN_LED = 0,
    LEFT_TURN_LED,
    RIGHT_TURN_LED,
    ALL_TURN_LED,
} timer_owner_e;

typedef enum{
    TIMER_STOP = 0,
    TIMER_RUN,
} timer_state_e;


struct gpio_turn_priv {

    int blink_value;
    unsigned long blink_delay_ms;
    timer_state_e tstate;
    timer_owner_e towner;
    struct timer_list gtimer;

    spinlock_t lock;

    dev_t devt;
    struct cdev cdev;
    struct class *class;
    struct device *device;

    struct gpio_desc *left_gpio;
    struct gpio_desc *right_gpio;
};

static void gpio_timer_handler(struct timer_list *timer)
{
    struct gpio_turn_priv *priv = container_of(timer, struct gpio_turn_priv, gtimer);
    unsigned long flags;
    unsigned long delay = 0;
    int value = 0;
    timer_owner_e tower;
    bool rerun = false;

    spin_lock_irqsave(&priv->lock, flags);
    value = priv->blink_value;
    tower = priv->towner;

    if(tower == ALL_TURN_LED)
        priv->blink_value = !priv->blink_value;

    if (priv->tstate == TIMER_RUN) {
        delay = priv->blink_delay_ms;
        rerun = true;
    }
    spin_unlock_irqrestore(&priv->lock, flags);

    switch (tower) {
    case LEFT_TURN_LED:
        gpiod_set_value(priv->left_gpio, !gpiod_get_value(priv->left_gpio));
        break;
    case RIGHT_TURN_LED:
        gpiod_set_value(priv->right_gpio, !gpiod_get_value(priv->right_gpio));
        break;
    case ALL_TURN_LED:
        gpiod_set_value(priv->left_gpio, value);
        gpiod_set_value(priv->right_gpio, value);
        break;
    default:
        break;
    }

    if (rerun)
        mod_timer(&priv->gtimer, jiffies + msecs_to_jiffies(delay));
}

static int gpio_turn_open(struct inode *inode, struct file *file){
    struct gpio_turn_priv *priv = container_of(inode->i_cdev, struct gpio_turn_priv, cdev);
    file->private_data = priv;
    return 0;
}

static long gpio_turn_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
    struct gpio_turn_priv *priv  = file->private_data;
    unsigned long data;
    unsigned long flags;
    long ret = 0;

    switch(cmd)
    {
        case GPIO_TURN_LEFT_SET:
        {
            if (copy_from_user(&data, (void __user *)arg, sizeof(data))){
                ret = -EFAULT;
                goto fault;
            }
                
            spin_lock_irqsave(&priv->lock,flags);
            if(priv->towner == NONE_TURN_LED)
                gpiod_set_value(priv->left_gpio,data);
            spin_unlock_irqrestore(&priv->lock,flags);
            break;
        }
        case GPIO_TURN_RIGHT_SET:
        {
            if (copy_from_user(&data, (void __user *)arg, sizeof(data))){
                ret = -EFAULT;
                goto fault;
            }
                
            spin_lock_irqsave(&priv->lock,flags);
            if(priv->towner == NONE_TURN_LED)
                gpiod_set_value(priv->right_gpio,data);
            spin_unlock_irqrestore(&priv->lock,flags);
            break;
        }
        case GPIO_TURN_LEFT_BLINK_START:
        {
            if (copy_from_user(&data, (void __user *)arg, sizeof(data))){
                ret = -EFAULT;
                goto fault;
            }
            if (data == 0){
                ret = -EINVAL;
                goto fault;
            }

            spin_lock_irqsave(&priv->lock,flags);
            if(priv->towner == NONE_TURN_LED){
                priv->blink_delay_ms = data;
                priv->towner = LEFT_TURN_LED;
                priv->tstate = TIMER_RUN;
                mod_timer(&priv->gtimer, jiffies + msecs_to_jiffies(data));    
            }
            spin_unlock_irqrestore(&priv->lock,flags);
            break;
        }
        case GPIO_TURN_LEFT_BLINK_STOP:
        {
            bool do_del = false;
            spin_lock_irqsave(&priv->lock,flags);
            if(priv->towner == LEFT_TURN_LED){               
                priv->towner = NONE_TURN_LED;
                priv->tstate = TIMER_STOP;
                priv->blink_delay_ms = 0;
                do_del = true;
            }
            spin_unlock_irqrestore(&priv->lock,flags);
            if(do_del){
                del_timer_sync(&priv->gtimer);
                gpiod_set_value(priv->left_gpio, 0);
            }
            break;
        }
        case GPIO_TURN_RIGHT_BLINK_START:
        {
            if (copy_from_user(&data, (void __user *)arg, sizeof(data))){
                ret = -EFAULT;
                goto fault;
            }
            if (data == 0){
                ret = -EINVAL;
                goto fault;
            }

            spin_lock_irqsave(&priv->lock,flags);
            if(priv->towner == NONE_TURN_LED){
                priv->blink_delay_ms = data;
                priv->towner = RIGHT_TURN_LED;
                priv->tstate = TIMER_RUN;
                mod_timer(&priv->gtimer,jiffies + msecs_to_jiffies(priv->blink_delay_ms));
            }
            spin_unlock_irqrestore(&priv->lock,flags);
            break;
        }
        case GPIO_TURN_RIGHT_BLINK_STOP:
        {
            bool do_del = false;
            spin_lock_irqsave(&priv->lock,flags); 
            if(priv->towner == RIGHT_TURN_LED){
                priv->towner = NONE_TURN_LED;
                priv->tstate = TIMER_STOP;
                priv->blink_delay_ms = 0;
                do_del = true;
            }
            spin_unlock_irqrestore(&priv->lock,flags);
            if(do_del){
                del_timer_sync(&priv->gtimer);
                gpiod_set_value(priv->right_gpio, 0);
            }
            break;
        }
        case GPIO_TURN_ALL_BLINK_START:
        {
            if (copy_from_user(&data, (void __user *)arg, sizeof(data))){
                ret = -EFAULT;
                goto fault;
            }
            if (data == 0){
                ret = -EINVAL;
                goto fault;
            }

            spin_lock_irqsave(&priv->lock,flags);
            if(priv->towner == NONE_TURN_LED){ 
                priv->blink_value = 0;
                priv->blink_delay_ms = data;
                priv->towner = ALL_TURN_LED;
                priv->tstate = TIMER_RUN;
                mod_timer(&priv->gtimer,jiffies + msecs_to_jiffies(priv->blink_delay_ms));          
            } 
            spin_unlock_irqrestore(&priv->lock,flags);
            break;    
        }
        case GPIO_TURN_ALL_BLINK_STOP:
        {
            bool do_del = false;
            spin_lock_irqsave(&priv->lock,flags);
            if(priv->towner == ALL_TURN_LED){                
                priv->blink_delay_ms = 0;
                priv->blink_value = 0;
                priv->towner = NONE_TURN_LED;
                priv->tstate = TIMER_STOP;
                do_del = true;    
            }
            spin_unlock_irqrestore(&priv->lock,flags);
            if(do_del){
                del_timer_sync(&priv->gtimer);
                gpiod_set_value(priv->left_gpio, 0);
                gpiod_set_value(priv->right_gpio, 0); 
            }
            break; 
        }             
        default:
            ret = -EINVAL;
            goto fault;
    }

fault:
    return ret;    
}

static int gpio_turn_release(struct inode *inode, struct file *file){
    pr_info("gpio_turn release\n");
    return 0;
}

static const struct file_operations gpio_turn_fops = {
    .owner = THIS_MODULE,
    .open = gpio_turn_open,
    .release = gpio_turn_release,
    .unlocked_ioctl = gpio_turn_ioctl,
};

static int gpio_turn_probe(struct platform_device *pdev){
    int ret;
    struct gpio_turn_priv *priv;
    struct device *dev = &pdev->dev;
    struct device_node *left_np,*right_np;

    dev_info(dev, "gpio_turn_probe start\n");
    priv = devm_kzalloc(dev,sizeof(*priv),GFP_KERNEL);
    if(priv == NULL){
        dev_err(dev, "devm_kzalloc failed\n");
        ret = -ENOMEM;
        goto err_alloc;
    }

    left_np = of_get_child_by_name(dev->of_node,"led-turnleft");
    if(left_np == NULL){
        ret = -ENODEV;
        dev_err(dev, "find left_np error\n");
        goto err_alloc;
    }
    right_np = of_get_child_by_name(dev->of_node,"led-turnright");
    if(right_np == NULL){
        ret = -ENODEV;
        dev_err(dev, "find right_np error\n");
        of_node_put(left_np);
        goto err_alloc;
    }

    priv->left_gpio = gpiod_get_from_of_node(left_np,"led-gpios",0,GPIOD_OUT_LOW,"turn-left");
    if (IS_ERR(priv->left_gpio)) {
        ret = PTR_ERR(priv->left_gpio);
        dev_err(dev, "find left_gpio error\n");
        goto err_nodes;
    }
    priv->right_gpio = gpiod_get_from_of_node(right_np,"led-gpios",0,GPIOD_OUT_LOW,"turn-right");
    if (IS_ERR(priv->right_gpio)) {
        ret = PTR_ERR(priv->right_gpio);
        dev_err(dev, "find right_gpio error\n");
        goto err_left_gpio;
    }

    ret = alloc_chrdev_region(&priv->devt, 0, 1, "gpio_turn");
    if (ret)
        goto err_right_gpio;

    cdev_init(&priv->cdev, &gpio_turn_fops);
    ret = cdev_add(&priv->cdev, priv->devt, 1);
    if (ret)
        goto err_chrdev;

    priv->class = class_create(THIS_MODULE, "gpio_turn_class");
    if (IS_ERR(priv->class)) {
        ret = PTR_ERR(priv->class);
        goto err_cdev;
    }

    priv->device = device_create(priv->class, NULL, priv->devt, NULL, "gpio_turn");
    if (IS_ERR(priv->device)) {
        ret = PTR_ERR(priv->device);
        goto err_class;
    }

    priv->blink_value = 0;
    priv->blink_delay_ms = 0;
    priv->towner = NONE_TURN_LED;
    priv->tstate = TIMER_STOP;
    spin_lock_init(&priv->lock);
    timer_setup(&priv->gtimer,gpio_timer_handler,0);
    platform_set_drvdata(pdev,priv);

    of_node_put(left_np);
    of_node_put(right_np);
    return 0;

err_class:
    class_destroy(priv->class);
err_cdev:
    cdev_del(&priv->cdev);
err_chrdev:
    unregister_chrdev_region(priv->devt, 1);
err_right_gpio:
    gpiod_put(priv->right_gpio);
err_left_gpio:
    gpiod_put(priv->left_gpio);
err_nodes:
    of_node_put(right_np);
    of_node_put(left_np);
err_alloc:
    return ret;
}

static int gpio_turn_remove(struct platform_device *pdev){
    struct gpio_turn_priv *priv = platform_get_drvdata(pdev);
    del_timer_sync(&priv->gtimer);
    gpiod_put(priv->left_gpio);
    gpiod_put(priv->right_gpio);
    device_destroy(priv->class, priv->devt);
    class_destroy(priv->class);
    cdev_del(&priv->cdev);
    unregister_chrdev_region(priv->devt, 1);
    pr_info("gpio_turn_remove\n");
    return 0;
}

static const struct of_device_id gpio_turn_of_match[] = {
    { .compatible = "myvendor,led-turn", },
    { /* sentinel */ }
};

static struct platform_driver gpio_turn_driver = {
    .driver = {
        .of_match_table = of_match_ptr(gpio_turn_of_match),
        .name = "gpio_turn",
        .owner = THIS_MODULE,
    },
    .probe = gpio_turn_probe,
    .remove = gpio_turn_remove,
};

static int __init gpio_turn_init(void){
    
    return platform_driver_register(&gpio_turn_driver);
}

static void __exit gpio_turn_exit(void){
    platform_driver_unregister(&gpio_turn_driver);
}

module_init(gpio_turn_init);
module_exit(gpio_turn_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wayne");
MODULE_DESCRIPTION("GPIO Turn Driver");
MODULE_VERSION("0.1");

