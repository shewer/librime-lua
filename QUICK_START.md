# engine process [ProcessKey(const KeyEvent & key_event)](https://github.com/rime/librime/blob/99e269c8eb251deddbad9b0d2c4d965b228f8006/src/rime/engine.cc#L95)
```
   foreach (processor in processors) {
         ret= processor->ProcessKeyEvent(key_event)
         if (ret== Rejected ) break;
         if (ret== Accepted) return true
         if (ret== Noop)  continue
   }
```
## 當觸發 OnContextUpdate時 call [engine->Compose(Context* ctx)](https://github.com/rime/librime/blob/99e269c8eb251deddbad9b0d2c4d965b228f8006/src/rime/engine.cc#L159)
* 處理 Segmentors 產生 [Composition](https://github.com/rime/librime/blob/99e269c8eb251deddbad9b0d2c4d965b228f8006/src/rime/composition.h#L21)
  :public [Segmention](https://github.com/rime/librime/blob/99e269c8eb251deddbad9b0d2c4d965b228f8006/src/rime/segmentation.h#L59)
  
   Segmetion :public vector<[Segment](https://github.com/rime/librime/blob/99e269c8eb251deddbad9b0d2c4d965b228f8006/src/rime/segmentation.h#L18)>
*  Composition comp 調出 segment 輪循 translator 
  * translator 產生 translation 加入 segment.menu
  * filter      
