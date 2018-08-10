
/**
******************************************************************************
* @file   webpge.h
* @author  WIZnet Software Team 
* @version V1.0
* @date    2015-xx-xx
* @brief   web服务器现在的内容
* @attention  
******************************************************************************
**/
#ifndef _WEBPAGE_H
#define _WEBPAGE_H

#include "eeprom.h"




#define LOGIN_HTML  "<!DOCTYPE html>"\
"<html>"\
"<title>山东华夏高科信息股份有限公司-串口服务器(微信)</title>"\
"<head>"\
"<title>Insert title here</title>"\
"</head>"\
"<body>"\
"<center>"\
"<form action='login.cgi' name='login' method='post'>"\
"<meta http-equive=\"Content-Type\" content=\"text/html; charset=gb2312\"/>"\
"<meta http-equive=\"Content-Language\" content=\"zh-CN\"/>"\
"<style>"\
"body{background:#B4C990;text-align:center}"\
"div{width:78px; margin:0 auto;background:#fff;text-align:left;}"\
"</style>"\
"<table>"\
"<h2>"\
"<br>"\
"</br>"\
"<font color = \"BLUE\">华夏高科串口服务器设置界面(微信)</td>"\
"</h2>"\
"<h3>"\
"<a href = \"http://www.buy365.com.cn\">华夏高科官方网站</a>"\
"</h3>"\
"<tr>"\
"<td>账&nbsp;&nbsp;号:</td>"\
"<td><input type = 'text' name = 'user' value = 'admins' size = '15' maxlength = \"6\"/></td>"\
"</tr>"\
"<tr>"\
"<td>密&nbsp;&nbsp;码:</td>"\
"<td><input type = 'password' name = 'password' size = '15' maxlength = \"6\"/></td>"\
"</tr>"\
"<tr>"\
"<td><input type = 'submit' value = '登&nbsp;&nbsp;录' size = '20'></td>"\
"</tr>"\
"</table>"\
"</form>"\
"</center>"\
"</body>"\
"</html>"\


#define INDEX_HTML  "<!DOCTYPE html>"\
"<head> "\
"<title>华夏高科串口服务器(微信)</title>"\
"<meta http-equiv='Content-Type' content='text/html; charset=GB2312' />"\
"<meta http-equiv='pragma' content='no-cache' /> "\
"<meta http-equiv='content-type' content='no-cache, must-revalidate' />"\
"<style> "\
"input[type='text'] {width:120px;}"\
"input[type='textn']{width:149px;}"\
"input[type='textg']{width:200px;}"\
"input[type='textl']{width:240px;}"\
"body{background-color:transparent;}"\
"body,h3,p,div{margin:0;padding:0;font: normal 15px 'omnes-pro', Helvetica, Arial, sans-serif;} "\
"#header {"\
"position: relative;"\
"margin: auto;"\
"}"\
"#header a {"\
"margin-left: 30px;"\
"}"\
"#header h2 {"\
"vertical-align: middle;"\
"font-size: 32px;"\
"font-weight: bold;"\
"text-decoration: none;"\
"color: #000;"\
"margin-left: 10px;"\
"text-align: center;"\
"}"\
".usual {"\
"background:transparent;"\
"color:#111;"\
"padding:15px 20px;"\
"width:auto;"\
"margin:8px auto;"\
"}"\
".usual li {list-style:none; float:left;}"\
".usual ul a {"\
"display:block;"\
"padding:6px 10px;"\
"text-decoration:none!important;"\
"margin:1px; "\
"margin-left:0;"\
"font-weight:bold;"\
"color:#FFF; "\
"background:#aaa;"\
"}"\
" .usual ul a:hover {"\
"color:#111;"\
"background:#EEF3FF;"\
"}"\
".usual ul a.selected { "\
"margin-bottom:0;"\
"color:#000;"\
"background:#EEF3FF;"\
"border-bottom:1px solid snow;"\
"cursor:default;"\
"}"\
".usual div{"\
"padding: 10px 10px 8px 10px;"\
"*padding-top:3px;"\
"*margin-top:15px;"\
"clear:left;"\
"background:#EEF3FF;"\
"}"\
".usual div a {color:#000; font-weight:bold;}"\
".hide{display:none;}"\
".show{display:block;}"\
".mr5{margin-right:5px;}"\
".red{color:red}"\
".usual div h3{margin-top:10px;margin-left:10px;font-size:20px;text-decoration:none;font-weight:bold;color:black;}"\
".usual div p{margin-top:0;margin-bottom:10px;} "\
".ipt label{float:left;padding-top:3px;text-align:left;width:100px;line-height:14px;}"\
".pwd label{float:left;margin-left:-5px;padding-top:3px;text-align:left;width:160px;}"\
".pointer{cursor:pointer;} "\
".ain{width:0%;height:100%;text-align:center;background:red;float:left;display:block;} "\
".conf-menu span {float:left;}"\
".conf-menu label{float:left; padding-top:3px; text-align:center; vertical-align:middle; width:70px;}"\
".conf-menu ul{margin-top:5px;margin-bottom:600px;} "\
".conf-menu li{padding-left:20px;padding-right:20px;list-style:none; float:left; text-align:center; font-weight:bold; clear:both}"\
".info label{float:left;text-align:left;width:100px;} "\
".info li{text-align:center;clear:both}"\
".info span{vertical-align:middle;float:left;text-align:left;display:block;padding:0px;border:0px;margin:0px;}"\
".info div{margin:0 auto;text-align:center;display:table-cell;vertical-align:middle;padding:0px 15px 10px 0px;background:#FFE4E1;}"\
"a.tooltip {outline:none; }"\
"a.tooltip strong {line-height:30px;}"\
"a.tooltip:hover {text-decoration:none;} "\
"a.tooltip span {"\
"z-index:10;display:none; padding:14px 20px;"\
"margin-top:-30px; margin-left:28px;"\
"width:150px; line-height:16px;"\
"}"\
"a.tooltip:hover span{"\
"display:inline; position:absolute; color:#111;"\
"border:1px solid #DCA; background:#fffAF0;}"\
".callout {z-index:20;position:absolute;top:30px;border:0;left:-12px;}"\
"a.tooltip span"\
"{"\
"border-radius:4px;"\
"box-shadow: 5px 5px 8px #CCC;"\
"}"\
" .fieldset{width:500px;font-size: 16px;font-weight: bold;}"\
" input[disabled='disabled']{border:none;background-color: transparent;}"\
" label{font-size:15px;}"\
"</style> "\
"<script> "\
"var tabLinks=new Array(); "\
"var divContents=new Array(); "\
"var t_UpSysdate;  "\
"var timer_is_on=0; "\
"var Net_timer_is_on=0; "\
"function init() "\
"{"\
"var tabListItems=document.getElementById('tabs').childNodes; "\
"for(var i=0;i<tabListItems.length;i++) "\
"{ "\
"if(tabListItems[i].nodeName=='LI') "\
"{ "\
"var tabLink=getFirstChildWithTagName(tabListItems[i],'A'); "\
"var id=getHash(tabLink.getAttribute('tabid')); "\
"tabLinks[id]=tabLink; "\
"divContents[id]=document.getElementById(id); "\
"}"\
"}"\
"var i=0;"\
"for(var id in tabLinks)"\
"{ "\
"tabLinks[id].onclick=showTab;"\
"if(i==0){tabLinks[id].className='selected';}"\
"i++; "\
"}"\
"};"\
"function showTab()"\
"{"\
"var selectedId=getHash(this.getAttribute('tabid'));"\
"for(var id in divContents)"\
"{"\
"if(id==selectedId)"\
"{ "\
"tabLinks[id].className='selected'; "\
"if(id)divContents[id].className='show'; "\
"if(selectedId=='tabs1'){DoUpNetdata();}else{StopUpNetdata();}"\
"if(selectedId=='tabs2'){;}else{;}"\
"}"\
"else"\
"{"\
"tabLinks[id].className='';"\
"if(id)divContents[id].className='hide';"\
"}"\
"}"\
"return false;"\
"};"\
"function getFirstChildWithTagName(element,tagName)"\
"{"\
"for(var i=0; i<element.childNodes.length;i++)"\
"{"\
"if(element.childNodes[i].nodeName==tagName) return element.childNodes[i];"\
"}"\
"};"\
"function getHash(url)"\
"{"\
"var hashPos=url.lastIndexOf('#');"\
"return url.substring(hashPos+1); "\
"}; "\
" function AJAX(url, callback)"\
"{"\
"var req = AJAX_init();"\
"req.onreadystatechange = AJAX_processRequest;"\
"function AJAX_init() {"\
"if (window.XMLHttpRequest) {"\
"return new XMLHttpRequest(); "\
"} else if (window.ActiveXObject) {"\
"return new ActiveXObject('Microsoft.XMLHTTP');"\
"}"\
"};"\
"function AJAX_processRequest() {"\
"if(req.readyState == 4) {"\
"if(req.status == 200) {"\
"if(callback) "\
"callback(req.responseText);"\
"}"\
"}"\
"};"\
"this.doGet = function() {"\
"req.open('GET', url, true);"\
"req.send(null);"\
"};"\
"this.doPost = function(body) {"\
"req.open('POST', url, true); "\
"req.setRequestHeader('Content-Type',"\
"'application/x-www-form-urlencoded');"\
"req.setRequestHeader('ISAJAX','yes');"\
"req.send(body); "\
"};"\
"};"\
"function $(id)  {return document.getElementById(id);}"\
"function $$(id){return document.getElementsByName(id);}"\
"function G(id){return $(id).style.color='green';} "\
"function R(id){return $(id).style.color='red';} "\
"function $$_ie(tag, name){"\
"if(!tag){tag='*';}"\
"var elems=document.getElementsByTagName(tag);"\
"var res=[];"\
"for(var i=0;i<elems.length;i++){"\
"att=elems[i].getAttribute('name');"\
"if(att==name){"\
"res.push(elems[i]);"\
"}"\
"}"\
"return res;"\
"} "\
"function selset(id,val){"\
"var o=$(id);"\
"for(var i=0;i<o.options.length;i++){"\
"if(i==val){"\
"o.options[i].selected=true;"\
"break;"\
"}"\
"}"\
"}"\
"function selset_name(name, val){"\
"var o=$$(name);"\
"for(var i=0;i<o.options.length;i++){"\
"if(i==val){"\
"o.options[i].selected=true;"\
"break;"\
"}"\
"}"\
"}"\
"function Callback(o){"\
"var obj;"\
"if (o.dhcp  == '0')"\
"{"\
"$('rdStatic').checked='checked'; "\
"}else { if($('rdDhcp'))$('rdDhcp').checked='checked';"\
"if($('txtIp')) $('txtIp').disabled  ='disabled'; "\
"if($('txtSub'))$('txtSub').disabled ='disabled'; "\
"if($('txtGw')) $('txtGw').disabled  ='disabled'; "\
"}"\
"if ($('txtMac')) $('txtMac').value= o.mac;"\
"if ($('txtIp'))  $('txtIp').value= o.ip;"\
"if ($('txtSub')) $('txtSub').value= o.sub;"\
"if ($('txtGw'))  $('txtGw').value= o.gw;"\
"if ($('txtDns')) $('txtDns').value= o.dns;"\
"if ($('txtport'))$('txtport').value= o.port;"\
"if ($('txtbaud'))$('txtbaud').value= o.baud;"\
"if (o.Rdns == '0')"\
"{"\
"if($('DomainNameIP'))$('DomainNameIP').checked='checked';"\
"}else "\
"{"\
"if($('DomainName'))$('DomainName').checked ='checked'; "\
"if($('txtRip'))$('txtRip').disabled='disabled'; "\
"}"\
"if($('txtRip'))$('txtRip').value= o.Rip;"\
"if($('txtRport'))$('txtRport').value = o.Rport;"\
"if($('txtRurl'))$('txtRurl').value= o.Rurl;"\
"netinfo_DomainName(o.Rdns);"\
"} "\
"function netinfo_block(o) {"\
"if(o =='1') {"\
"if ($('txtIp')) $('txtIp').disabled  ='disabled';"\
"if ($('txtSub'))$('txtSub').disabled ='disabled';"\
"if ($('txtGw')) $('txtGw').disabled  ='disabled';"\
"} else {"\
"if ($('txtIp')) $('txtIp').disabled=false;"\
"if ($('txtSub'))$('txtSub').disabled=false;"\
"if ($('txtGw')) $('txtGw').disabled=false;"\
"}"\
"}"\
"function netinfo_DomainName(o) {"\
"if($('txtRport')) $('txtRport').parentNode.style.display = 'block';"\
"if(o == '1') {"\
"if ($('txtRip'))$('txtRip').disabled = 'disabled';"\
"if ($('txtRip'))$('txtRip').parentNode.style.display = 'none';"\
"if ($('txtRurl'))$('txtRurl').parentNode.style.display = 'block';"\
"} else {"\
"if ($('txtRip'))$('txtRip').disabled = false;"\
"if ($('txtRip'))$('txtRip').parentNode.style.display = 'block';"\
"if ($('txtRurl'))$('txtRurl').parentNode.style.display = 'none';"\
"} "\
"}"\
"function UpSysdata(){"\
"var oUpdate; "\
"setTimeout(function(){"\
"oUpdate=new AJAX('status.cgi',function(t){"\
"try{eval(t);}catch(e){location.href='/';}"\
"});	 	 "\
"oUpdate.doGet();},50);"\
"t_UpSysdate=setTimeout('UpSysdata()',1000); "\
"} "\
"function DoUpSysdata(){"\
"if(!timer_is_on) "\
"{ "\
"timer_is_on=1; "\
"UpSysdata(); "\
"} "\
"} "\
"function StopUpSysdata(){"\
"clearTimeout(t_UpSysdate);"\
"timer_is_on=0; "\
"}"\
"function UpNetdata(){ "\
"var oUpdate; "\
"oUpdate=new AJAX('config.cgi',function(t){"\
"try{eval(t);}catch(e){location.href='/';}"\
"});	 	 "\
"oUpdate.doGet();"\
"} "\
"function DoUpNetdata(){"\
"if(!Net_timer_is_on) "\
"{ "\
"Net_timer_is_on=1;"\
"UpNetdata(); "\
"}"\
" }"\
"function StopUpNetdata(){ "\
"Net_timer_is_on=0; "\
"}"\
"</script> "\
"</head> "\
"<body onload='init();'> "\
"<div id='header'> 	"\
"<center> <h2>华夏高科串口服务器(微信)</h2></center>"\
"</div>"\
"<div class='usual'>"\
"<ul id='tabs'> "\
"<li><a href='#NetSettings'   tabid='#tabs1' accesskey='1' onkeyup='DoUpSysdata();'>网络设置 </a></li> "\
"<li><a href='#AdminSetting'  tabid='#tabs2' accesskey='2'>管理员设置</a></li> "\
"</ul>"\
"<div id='tabs1' class='show'> "\
"<form id='frmParam' method='POST' action='config.cgi'> "\
"<div class='ipt'> "\
"<legend >本地网络参数</legend><p><label></label>"\
"<input name='dhcp' type='radio' id='rdStatic' value='0' onclick='netinfo_block(this.value);'/>手动分配IP&nbsp; &nbsp;"\
"<input name='dhcp' type='radio' id='rdDhcp'   value='1' onclick='netinfo_block(this.value);'/>自动获取IP"\
"<p><label>IP  地址: </label><input id='txtIp' name='ip' type='textn' size='25' maxlength = '15' value='' /></p>"\
"<p><label>子网掩码: </label><input id='txtSub'name='sub'type='textn' size='25' maxlength = '15' value='' /></p>"\
"<p><label>默认网关: </label><input id='txtGw' name='gw' type='textn' size='25' maxlength = '15' value='' /></p>"\
"<p><label>DNS服务器:</label><input id='txtDns'name='dns'type='textn' size='25' maxlength = '15' value='' /></p>"\
"<p><label>端口号:	 </label><input id='txtport' name='port' type='textn' size='25' maxlength = '5' value='' /></p>"\
"<p><label>波特率:	 </label><input id='txtbaud' name='baud' type='textn' size='25' maxlength = '10'value='8000000' /></p>"\
"</fieldset><br>"\
"<fieldset class='fieldset'>"\
"<legend>远程服务器网络参数</legend>"\
"<p><label></label>"\
"<input name='DomainName' type='radio' id='DomainName'  value='1' onclick='netinfo_DomainName(this.value);'/>服务器域名&nbsp; &nbsp; "\
"<input name='DomainName' type='radio' id='DomainNameIP'value='0' onclick='netinfo_DomainName(this.value);'/>服务器IP"\
"</p>"\
"<p><label>域名地址:</label><input id='txtRurl'	 name='Rurl'  type='textl' maxlength = '50' value='' /></p>"\
"<p><label>IP 地址: </label><input id='txtRip'	 name='Rip'   type='textl' maxlength = '15' value='' /></p>"\
"<p><label>端口号:	</label><input id='txtRport' name='Rport' type='textl' maxlength = '5'  value='' /></p>"\
"</fieldset><br>"\
"<p>"\
"<input name='NetSave' type='submit' class='btn' id='NetSave' value='保存并重启'/>"\
"</p>"\
"</div>"\
"</form>"\
"</div>"\
"<div id='tabs2' class='hide'> "\
"<form id='adminParam' method='post' action='admin.cgi'> "\
"<div class='ipt'> "\
"<fieldset class='fieldset'>"\
"<legend>用户信息管理</legend>"\
"<p><label>用户信息管理</label>"\
"<select style='height:21px;width:185px;' id='select' name='select' >"\
"<option value='0'>设备管理员</option>"\
"<option value='1'>MQTT管理员</option>"\
"</select> "\
"<p><label for='txtAdmin'>原用户名:</label><input type='text' placeholder='长度为6个数字、字母的组合' id='txtAdmin' name='admins' style='width:180px;' size='16' maxlength='6'/></p>"\
"<p><label for='txtPassword'>原始口令:</label><input type='password' placeholder='长度为6个数字、字母的组合' id='txtPassword' name='password' style='width:180px;'size='16' maxlength='6'/></p>"\
"<p><label for='txtAdmin'>新用户名:</label><input type='text' placeholder='长度为6个数字、字母的组合' id='txtAdmin' name='nadmin' style='width:180px;'size='16' maxlength='6'/></p>"\
"<p><label for='txtPassword'>新口令:</label><input type='password' placeholder='长度为6个数字、字母的组合' id='txtPassword' name='npassword'  style='width:180px;'size='16' maxlength='6'/></p>"\
"<p><label for='txtPassword'>确认新口令:</label><input type='password' placeholder='重新输入新口令' id='txtPassword' name='nnpassword' style='width:180px;'size='16' maxlength='6'/></p>"\
"</fieldset><br>"\
"<p>"\
"<input name='adminRst'  type='submit' class='btn' id='adminRst'  value='重启'/>&nbsp;&nbsp;&nbsp;&nbsp;"\
"<input name='adminSave' type='submit' class='btn' id='adminSave' value='保存'/> "\
"</p>"\
"</div> "\
"</form>"\
"</div> "\
"<div style='margin:5px 5px; clear:both' > "\
"<center>"\
"&copy;Copyright 2018 山东华夏高科信息股份有限公司"\
"</center>"\
"</div> "\
"<script type='text/javascript' src='w5500.js'></script>"\
"</body> "\
"</html>"

#endif
