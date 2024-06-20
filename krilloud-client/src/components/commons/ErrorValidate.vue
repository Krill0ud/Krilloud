<template>
  <div align="center" class="notify q-mt-md q-mb-md" v-if="message.length > 0" >
        <q-list justify-center v-for="m in message" :key="m.key">
           <q-item >
        <q-item-section>
          {{m}}
        </q-item-section>
      </q-item>
        </q-list>
  </div>
</template>

<script>
export default {
  props: { message: Array }
}
</script>
