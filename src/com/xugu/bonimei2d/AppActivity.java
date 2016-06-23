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

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;

public class AppActivity extends Cocos2dxActivity {
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		QLAdController.getInstance().init(this,R.drawable.icon, true);   
	}
	
	public static void fenxiang(int i)
	{
		Log.e("----------","fenxiang="+i);
	}
	
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			new AlertDialog.Builder(AppActivity.this)
					.setTitle("系统提示")
					// 设置对话框标题
					.setMessage("确定要离开吗？")
					// 设置显示的内容
					.setPositiveButton("确定",
							new DialogInterface.OnClickListener() {// 添加确定按钮
								@Override
								public void onClick(DialogInterface dialog,
										int which) {// 确定按钮的响应事件
									// TODO Auto-generated method stub
									finish();
								}
							})
					.setNegativeButton("返回",
							new DialogInterface.OnClickListener() {// 添加返回按钮
								@Override
								public void onClick(DialogInterface dialog,
										int which) {// 响应事件
									// TODO Auto-generated method stub
								}
							}).show();// 在按键响应事件中显示此对话框
			
			return true;
		}		
		return super.onKeyDown(keyCode, event);
		
	}

}
