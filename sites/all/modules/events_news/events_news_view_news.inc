<?php
function events_news_view_news($id){
    global $base_url;
    global $user;
    $decrypting_key=variable_get('IIUM_Encryption_Key', NULL);
    $id=(int)decrypt($id,$decrypting_key);
    $news = db_select('news')
        ->fields('news')
        ->condition('id',$id,'=')
         ->execute();
    
    $news_attachments = db_select('news_attachments')
        ->fields('news_attachments')
        ->condition('news_id',$id,'=')
         ->execute()
       ->fetchAll();
    
    
    $form = array();
    
    $form['event'] = array(
        '#type' => 'item', 
        '#title' => ''
    );
   
   $output = '<div class="block block-system clearfix view-news-events">';
    $reportedArr = reported();
    foreach($news as $row) {
        if($row->photo!='')
            $image = ' <img src="'.file_create_url($row->photo).'"/>';
        else
            $image = '<img src="'.file_create_url('public://uploads/profile_pictures/noimage.jpg').'"/>';
        $output .='<div class="EN-details-header"> 
               <span class="date-time">'.dateformat($row->date).' </span> 
        <h1>'.$row->title.'</h1> 
            <div class="EN-header-right">
               <label>News ID :</label>
               <span>'.sprintf("NW-%04d", $row->id).'</span>
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
        <p>'.$row->description.'</p>';
        if(count($news_attachments)>0)
        {
         $output .='<ul class="en-details-list">Attachment:';
        foreach($news_attachments as $res) {
            $output .='
                <li><a href="'.file_create_url($res->attachment).'" target="_blank">download</a>
             </li>';
        }
        $output .='</ul>';
        }
    }
    
    $output .= '</div>';
    
    $form['news']['list'] = array(
        '#type' => 'item',         
        '#markup' => $output,         
     ); 
    
    return $form;
}