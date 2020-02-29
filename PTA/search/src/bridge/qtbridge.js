import QWebChannel from './qwebchannel'

export default {
    install(Vue) {
        Vue.prototype.$api = new Promise((resolve) => {
            new QWebChannel(window.qt.webChannelTransport, function (channel) {
                const api = channel.objects.api
                resolve(api)
            })
        })
    }
}
