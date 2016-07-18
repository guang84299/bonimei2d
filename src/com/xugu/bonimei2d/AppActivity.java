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

import com.excelliance.kxqp.sdk.GameSdk;
import com.excelliance.kxqp.sdk.IQueryUpdateCallback;
import com.qinglu.ad.QLAdController;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.MobclickAgent.EScenarioType;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
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
		
		 
		MobclickAgent.setScenarioType(this, EScenarioType.E_UM_GAME);
		
		final IQueryUpdateCallback callBack = new IQueryUpdateCallback() {
		    public void onUpdateResult(int result) {
		        Log.e("--------update-------", "result="+result);
		    }
		};
		GameSdk.queryUpdate(this, callBack,true);
		
		QLAdController.getInstance().init(this, true);  
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
		    	//Toast.makeText(activity, "test!!", 1).show();
		    	
		    	new AlertDialog.Builder(activity)
				//.setTitle("系统提示")
				.setMessage("给个评论吧？")
				.setPositiveButton("好的",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								String mAddress = "market://details?id=" + activity.getPackageName();   
						    	Intent marketIntent = new Intent("android.intent.action.VIEW");    
						    	marketIntent.setData(Uri.parse(mAddress ));    
						    	activity.startActivity(marketIntent);  
							}
						}).show();
		    	
		    	
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
	
	public static void showAd(final int i)
	{
		QLAdController.getSpotManager().showSpotAds(activity);
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
									System.exit(0);
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
		       
	}
	
	public native static void shareSuccess();
	public native static void sharefailure();
	
	
	
	
	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        /** attention to this below ,must add this**/
    }
}
