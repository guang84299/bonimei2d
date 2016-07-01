package com.xugu.bonimei2d;

import android.app.Application;
import android.os.Bundle;

import com.umeng.fb.push.FeedbackPush;
import com.umeng.socialize.PlatformConfig;

/**
 * Created by ntop on 15/7/8.
 */
public class App extends Application {

    @Override
    public void onCreate() {
      FeedbackPush.getInstance(this).init(false);
        super.onCreate();
    }

    //各个平台的配置，建议放在全局Application或者程序入口
    {
        //微信 wx12342956d1cab4f9,a5ae111de7d9ea137e88a5e02c07c94d
        PlatformConfig.setWeixin("wx31d08c0693437c58", "efcdfccc6f036b5eed6f32473af3e2e9");
        //新浪微博
        //PlatformConfig.setSinaWeibo("3921700954", "04b48b094faeb16683c32669824ebdad");
        PlatformConfig.setQQZone("1105493910", "1Hp8c0kEzTR1ZeLo");
 
    }
}
