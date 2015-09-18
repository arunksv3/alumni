<?php

function events_news_news_details($form, &$form_state,$nid){
    global $base_url;
    global $user;
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $id=(int)decrypt($nid,$decrypting_key);
    $news = db_select('news')
        ->fields('news')
        ->condition('id',$id,'=')
        ->condition('status',1,'=')
        ->execute();
    
    $news_attachments = db_select('news_attachments')
        ->fields('news_attachments')
        ->condition('news_id',$id,'=')
         ->execute();
    
    
$form = array();
    
    $form['news'] = array(
        '#type' => 'item', 
        '#title' => ''
    );
    $output = '<div class="block block-system clearfix">';
    $reportedArr = reported();
    foreach($news as $row) {
        if($row->photo!='')
            $image = ' <img src="'.file_create_url($row->photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $output .='<div class="EN-details-header"> 
               <span class="date-time">'.date('d M Y',strtotime($row->date)).' </span> 
        <h1>'.$row->title.'</h1> 
            <div class="EN-header-right">
              
            </div>
        </div>
        <div class="EN-details-content">
        <ul class="en-details-list">
           <li>
        <label>Reported By:</label><span> '.$reportedArr[$row->reported_by].'</span> 
        </li>
        </ul>
        <div class="EN-content-img">
            '.$image.'
        </div> 
        <p>'.$row->description.'</p> 
        <ul class="en-details-list"><li>Attachment:';
        foreach($news_attachments as $res) {
            $output .='
                <a href="'.file_create_url($res->attachment).'" target="_blank">download</a>
             </li>';
        }
        $output .='</ul></div>';
    }
    
    $output .= '</div>';
    
    $form['news']['list'] = array(
        '#type' => 'item',         
        '#markup' => $output,         
     ); 
    
     return $form;
    
}