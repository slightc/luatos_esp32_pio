# pwm.open

```lua
pwm.open(channel, period, pulse)
```

开启指定的PWM通道

## 参数表

Name | Type | Description
-----|------|--------------
`channel`|`int`| PWM通道
`period`|`int`| 频率, 1-1000000hz
`pulse`|`int`| 占空比 0-100

## 返回值

> `boolean`: 处理结果,成功返回true,失败返回false

## 调用示例

```lua
-- 打开PWM1, 频率1kHz, 占空比50%
pwm.open(1, 1000, 50)
```


--------------------------------------------------
# pwm.close

```lua
pwm.close(channel)
```

关闭指定的PWM通道

## 参数表

Name | Type | Description
-----|------|--------------
`channel`|`int`| PWM通道

## 返回值

> *无返回值*

## 调用示例

```lua
-- 关闭PWM1
pwm.close(1)
```

