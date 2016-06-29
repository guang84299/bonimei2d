/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.xugu.bonimei2d;

import org.cocos2dx.lib.Cocos2dxActivity;
import com.qinglu.ad.QLAdController;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.MobclickAgent.EScenarioType;
import com.umeng.fb.ConversationActivity;
import com.umeng.fb.FeedbackAgent;
import com.umeng.socialize.ShareAction;
import com.umeng.socialize.UMShareListener;
import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.media.UMImage;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity {
	static Activity activity;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		activity = this;
		
		QLAdController.getInstance().init(this,R.drawable.icon, true);   
		MobclickAgent.setScenarioType(this, EScenarioType.E_UM_GAME);
	}
	
	@Override
	protected void onResume() {
		MobclickAgent.onResume(this);
		super.onResume();
	}
	
	@Override
	protected void onPause() {
		MobclickAgent.onPause(this);
		super.onPause();
	}
	
	public static void fenxiang(final int num)
	{
		activity.runOnUiThread(new Runnable() {		 
		    @Override
		    public void run() {
		    	share(num);
		    }
		});
	}
	
	public static void fankui()
	{
		activity.runOnUiThread(new Runnable() {		 
		    @Override
		    public void run() {
		    	FeedbackAgent agent = new FeedbackAgent(activity);
		    	agent.startFeedbackActivity();
		    }
		});
	}
	
	public static void level_guoguan()
	{
		MobclickAgent.onEvent(activity, "level_guoguan");
	}
	
	public static void level_suiji()
	{
		MobclickAgent.onEvent(activity, "level_suiji");
	}
	
	@SuppressLint("NewApi")
	public static void copyStr(final String str)
	{
		activity.runOnUiThread(new Runnable() {		 
		    @Override
		    public void run() {
		    	ClipboardManager cmb = (ClipboardManager)activity.getSystemService(activity.CLIPBOARD_SERVICE);  
				cmb.setPrimaryClip(ClipData.newPlainText(null, str));  
				
				Toast.makeText(activity, "已经复制内容到剪切板", 1).show();
		    }
		});
	}
	
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			new AlertDialog.Builder(AppActivity.this)
					.setTitle("系统提示")
					.setMessage("确定要离开吗？")
					.setPositiveButton("确定",
							new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									finish();
								}
							})
					.setNegativeButton("返回",
							new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {
								}
							}).show();
			
			return true;
		}		
		return super.onKeyDown(keyCode, event);
		
	}

	
	public static void share(int num)
	{
		UMImage image = new UMImage(activity, "http://120.25.87.115/images/others/bonimei2d-icon.png");

        String url = "http://120.25.87.115/apks/bonimei2d.apk";
        
        new ShareAction(activity).setDisplayList(SHARE_MEDIA.WEIXIN,SHARE_MEDIA.WEIXIN_CIRCLE,SHARE_MEDIA.QQ,SHARE_MEDIA.QZONE)        
        .withMedia(image)
        .withTitle("拯救火柴人")
        .withText("我已拯救"+num+"个火柴人，更多兄弟等你来救！")
        .withTargetUrl(url)
        .setCallback(umShareListener)
        .open();
       
	}
	
	private static UMShareListener umShareListener = new UMShareListener() {
        @Override
        public void onResult(SHARE_MEDIA platform) {
            Log.d("plat","platform"+platform);
            if(platform.name().equals("WEIXIN_FAVORITE")){
                Toast.makeText(activity,platform + " 收藏成功啦",Toast.LENGTH_SHORT).show();
            }else{
                Toast.makeText(activity, platform + " 分享成功啦", Toast.LENGTH_SHORT).show();
            }
        }

        @Override
        public void onError(SHARE_MEDIA platform, Throwable t) {
            Toast.makeText(activity,platform + " 分享失败啦", Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onCancel(SHARE_MEDIA platform) {
            Toast.makeText(activity,platform + " 分享取消了", Toast.LENGTH_SHORT).show();
        }
    };
}
